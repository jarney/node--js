#pragma once

#include "node--js/NodeModule.hpp"
#include "node--js/SerializerError.hpp"
#include <istream>
#include <ostream>

namespace NodeJS {
    namespace core {
	class SerializerErrorReporter;

	class ModuleLoaderPlugin {
	public:
	    virtual ~ModuleLoaderPlugin() = default;
	    
	    virtual std::unique_ptr<NodeModule> loadModule(
		std::string aFullyQualifiedModuleName,
		ModuleLoader & aModuleLoader,
		SerializerErrorReporter & reporter
		) = 0;
	};

	class ModuleLoaderPluginNil: public ModuleLoaderPlugin {
	public:
	    ModuleLoaderPluginNil() = default;
	    ~ModuleLoaderPluginNil() = default;
	    std::unique_ptr<NodeModule> loadModule(
		std::string aFullyQualifiedModuleName,
		ModuleLoader & aModuleLoader,
		SerializerErrorReporter & reporter
		);
	};
	
        /**
         * This is the base class for serializing node programs
         * to various formats.  This is typically implemented
	 * as a singleton which can be used in a number of
	 * places where a serializer is required to load or save
	 * the content of a module.
         */
	class ModuleLoader {
	public:
	    ModuleLoader(std::unique_ptr<ModuleLoaderPlugin> plugin);

	    /**
	     * Mainly used for testing...
	     */
	    ModuleLoader();

	    void setPlugin(std::unique_ptr<ModuleLoaderPlugin> plugin);
	    
	    /**
	     * This class is responsible for loading a module
	     * from its fully-qualified name.  The underlying
	     * assumption is usually that the name of the package
	     * lines up well with the name of the file on a filesystem
	     * and can be loaded that way.  It is also possible that
	     * the module is loaded from some storage medium other than
	     * a filesystem where it is indexed by package name.
	     *
	     * This function may block and may cache the results in memory
	     * to avoid loading the same module over again.
	     */
	    NodeModule *loadModule(
		std::string aFullyQualifiedModuleName,
		SerializerErrorReporter & reporter
		);

	    /**
	     * This creates a new (mutable)
	     * module so that new modules can be loaded and
	     * manipulated.  Generally speaking, tools
	     * only ever modify a single module at one time (in theory?)
	     */
	    NodeModule *newModule(
		std::string packageName
		);

	    void loadModule(std::unique_ptr<NodeModule> aModule);
	    
	    std::vector<std::string> getLoadedModules(void) const;
	    
	    bool hasModule(std::string aPackageNamespace);
	    
	    NodeModule *getLoadedModule(std::string moduleName) const;
	    
	private:
	    std::unique_ptr<ModuleLoaderPlugin> mPlugin;
	    std::map<std::string, std::unique_ptr<NodeModule>> mLoadedModules;
	};

    } // End core
} // End NodeJS
