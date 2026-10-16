#include "node--js/ModuleLoader.hpp"
#include "node--js/NodeModule.hpp"

#include <memory>

using namespace NodeJS::core;

ModuleLoader::ModuleLoader(
    std::unique_ptr<ModuleLoaderPlugin> aPlugin
    )
    : mPlugin(std::move(aPlugin))
{}

ModuleLoader::ModuleLoader()
    : mPlugin(std::make_unique<ModuleLoaderPluginNil>())
{}

void
ModuleLoader::setPlugin(std::unique_ptr<ModuleLoaderPlugin> plugin)
{
    mPlugin = std::move(plugin);

    // When we update the module loader,
    // we want to forget any past load failures
    // so that we can try again if we need to
    // resolve a module.
    std::vector<std::string> failedLoads;
    for (const auto & loadedModule : mLoadedModules) {
	if (loadedModule.second.get() == nullptr) {
	    failedLoads.push_back(loadedModule.first);
	}
    }
    
    for (auto failedLoad : failedLoads) {
	mLoadedModules.erase(failedLoad);
    }
}

NodeModule *
ModuleLoader::loadModule(
    std::string aFullyQualifiedModuleName,
    SerializerErrorReporter & reporter
    )
{
    // First, check if we already have it:
    const auto it = mLoadedModules.find(aFullyQualifiedModuleName);
    if (it != mLoadedModules.end()) {
	return it->second.get();
    }

    std::unique_ptr<NodeModule> nodeModule = mPlugin->loadModule(aFullyQualifiedModuleName, *this, reporter);
    NodeModule *ret = nodeModule.get();

    // Insert into map even if there was a failure
    // so we don't keep trying to load modules that
    // failed.
    mLoadedModules.insert(std::make_pair(aFullyQualifiedModuleName, std::move(nodeModule)));

    return ret;
}


NodeModule *
ModuleLoader::newModule(
    std::string aFullyQualifiedModuleName
    )
{
    const auto it = mLoadedModules.find(aFullyQualifiedModuleName);
    if (it == mLoadedModules.end()) {
	std::unique_ptr<NodeModule> nodeModule = std::make_unique<NodeModule>(*this, aFullyQualifiedModuleName);
	NodeModule *ret_module = nodeModule.get();
	mLoadedModules.insert(std::make_pair(aFullyQualifiedModuleName, std::move(nodeModule)));
	return ret_module;
    }
    return it->second.get();
}

#if NEED_MODULE_REFLECTION
void
ModuleLoader::loadModule(std::unique_ptr<NodeModule> aModule)
{
    std::string packageNamespace = aModule->getPackage();
    mLoadedModules.insert(std::make_pair(packageNamespace, std::move(aModule)));
}

std::vector<std::string>
ModuleLoader::getLoadedNamespaces(void) const
{
    std::vector<std::string> loadedNamespaces;

    for (const auto & it : mLoadedModules) {
	loadedNamespaces.push_back(it.first);
    }
    
    return loadedNamespaces;
}

bool
ModuleLoader::hasModule(std::string aPackageNamespace)
{
    return mLoadedModules.find(aPackageNamespace) != mLoadedModules.end();
}

NodeModule *
ModuleLoader::getLoadedModule(std::string moduleName) const
{
    const auto it = mLoadedModules.find(moduleName);
    if (it == mLoadedModules.end()) return nullptr;
    return it->second.get();
}
#endif

std::unique_ptr<NodeModule>
ModuleLoaderPluginNil::loadModule(
    std::string aFullyQualifiedModuleName,
    ModuleLoader & aModuleLoader,
    SerializerErrorReporter & reporter
    )
{
    reporter.reportError(
	SerializerErrorCode::NO_LOADER_DEFINED,
	0,
	aFullyQualifiedModuleName,
	"No module loader was defined, so we cannot load modules."
	);

    
    return nullptr;
}

