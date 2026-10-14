#include "node--js/NodeModule.hpp"
#include "node--js/QualifiedIdentifier.hpp"

using namespace NodeJS::core;

NodeModule::NodeModule(ModuleLoader & aModuleLoader, std::string aFullyQualifiedModuleName)
    : mModuleLoader(aModuleLoader)
    , mPackage(aFullyQualifiedModuleName)
    , mDescription("Uncategorized")
{}

void
NodeModule::setPackage(std::string aPackage)
{
    mPackage = aPackage;
}

std::string
NodeModule::getPackage(void) const
{
    return mPackage;
}

void
NodeModule::setDescription(std::string aDescription)
{
    mDescription = aDescription;
}

std::string
NodeModule::getDescription() const
{
    return mDescription;
}


ModuleLoader &
NodeModule::getModuleLoader(void) const
{
    return mModuleLoader;
}
	    

void
NodeModule::addDataType(std::unique_ptr<DataType> dataType)
{
    mDataTypes.insert(std::make_pair(dataType->getId(), std::move(dataType)));
}

void
NodeModule::removeDataType(std::string name)
{
    mDataTypes.erase(name);
}

const
std::map<std::string, std::unique_ptr<DataType>> &
NodeModule::getDataTypes() const
{
    return mDataTypes;
}

bool
NodeModule::hasDataType(const std::string & name) const
{
    QualifiedIdentifier qualifiedName(name);
    if (qualifiedName.isQualified() && qualifiedName.getNamespace() != mPackage) {
	// Search the scopes.
	NodeModule *searchedModule = getLoadedModule(qualifiedName.getNamespace());
	if (searchedModule == nullptr) return false;
	return searchedModule->hasDataType(qualifiedName.getName());
    }
    else {
	return mDataTypes.count(qualifiedName.getName()) != 0;
    }
}

bool
NodeModule::hasNodeType(const std::string & name) const
{
    QualifiedIdentifier qualifiedName(name);
    if (qualifiedName.isQualified() && qualifiedName.getNamespace() != mPackage) {
	// Search the scopes.
	NodeModule *searchedModule = getLoadedModule(qualifiedName.getNamespace());
	if (searchedModule == nullptr) {
	    fprintf(stderr, "Could not find module from fully qualified name %s", qualifiedName.getNamespace().c_str());
	    return false;
	}
	fprintf(stderr, "Found module, just can't find type\n");
	return searchedModule->hasNodeType(qualifiedName.getName());
    }
    else {
	return mNodeTypes.count(qualifiedName.getName()) != 0;
    }
}

const DataType *
NodeModule::getDataType(const std::string & name) const
{
    QualifiedIdentifier qualifiedName(name);
    if (qualifiedName.isQualified() && qualifiedName.getNamespace() != mPackage) {
	// Search the scopes.
	NodeModule *searchedModule = getLoadedModule(qualifiedName.getNamespace());
	if (searchedModule == nullptr) {
	    fprintf(stderr, "Could not find module from fully qualified name %s", qualifiedName.getNamespace().c_str());
	    return nullptr;
	}
	fprintf(stderr, "Found module, just can't find type\n");
	return searchedModule->getDataType(qualifiedName.getName());
    }
    else {
	const auto & it = mDataTypes.find(qualifiedName.getName());
	if (it == mDataTypes.end()) {
	    return nullptr;
	}
	return it->second.get();
    }
}

/////////////////////////////////////
void
NodeModule::addNodeType(std::unique_ptr<NodeType> nodeType)
{
    mNodeTypes.insert(std::make_pair(nodeType->getId(), std::move(nodeType)));
}

void
NodeModule::removeNodeType(std::string name)
{
    mNodeTypes.erase(name);
}

const NodeType *
NodeModule::getNodeType(const std::string & name) const
{
    QualifiedIdentifier qualifiedName(name);
    if (qualifiedName.isQualified() && qualifiedName.getNamespace() != mPackage) {
	// Search the scopes.
	NodeModule *searchedModule = getLoadedModule(qualifiedName.getNamespace());
	if (searchedModule == nullptr) {
	    fprintf(stderr, "Could not find module from fully qualified name %s", qualifiedName.getNamespace().c_str());
	    return nullptr;
	}
	fprintf(stderr, "Found module, just can't find type\n");
	return searchedModule->getNodeType(qualifiedName.getName());
    }
    else {
	const auto & it = mNodeTypes.find(qualifiedName.getName());
	if (it == mNodeTypes.end()) {
	    return nullptr;
	}
	return it->second.get();
    }
}

const
std::map<std::string, std::unique_ptr<NodeType>> &
NodeModule::getNodeTypes() const
{
    return mNodeTypes;
}

NodeGraph *
NodeModule::addGraph(std::string id)
{
    if (mGraphs.find(id) != mGraphs.end()) {
	return nullptr;
    }
    std::unique_ptr<NodeGraph> graph = std::make_unique<NodeGraph>(*this);
    NodeGraph *ret = graph.get();
    mGraphs.insert(std::pair(id, std::move(graph)));
    return ret;
}

NodeGraph *
NodeModule::getGraph(std::string id) const
{
    const auto & it = mGraphs.find(id);
    if (it == mGraphs.end()) {
	return nullptr;
    }
    return it->second.get();
}

bool
NodeModule::hasGraph(std::string id) const
{
    return mGraphs.find(id) != mGraphs.end();
}

const std::map<std::string, std::unique_ptr<NodeGraph>> &
NodeModule::getGraphs() const
{
    return mGraphs;
}

const Metadata &
NodeModule::getMetadata(void) const
{
    return mMetadata;
}

Metadata &
NodeModule::getMetadata(void)
{
    return mMetadata;
}
void
NodeModule::loadModule(std::unique_ptr<NodeModule> aModule)
{
    std::string packageNamespace = aModule->getPackage();
    mImportedModules.insert(std::make_pair(packageNamespace, std::move(aModule)));
}

std::vector<std::string>
NodeModule::getLoadedNamespaces(void) const
{
    std::vector<std::string> loadedNamespaces;

    for (const auto & it : mImportedModules) {
	loadedNamespaces.push_back(it.first);
    }
    
    return loadedNamespaces;
}

bool
NodeModule::hasModule(std::string aPackageNamespace)
{
    return mImportedModules.find(aPackageNamespace) != mImportedModules.end();
}

NodeModule *
NodeModule::getLoadedModule(std::string moduleName) const
{
    const auto it = mImportedModules.find(moduleName);
    if (it == mImportedModules.end()) return nullptr;
    return it->second.get();
}

