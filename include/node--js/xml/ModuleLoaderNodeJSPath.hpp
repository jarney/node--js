#pragma once

#include "node--js/ModuleLoader.hpp"

namespace NodeJS {
    namespace core {
	class NodeModule;
    }
    namespace xml {

#if defined(__WINDOWS__) || defined(__WIN32__) || defined(_WIN32) || defined(__CYGWIN__)
        static const char NODEJS_PATH_DELIMITER_CHARACTER = ';';
        static const std::string NODEJS_PATH_DELIMITER_STRING(";");
#else
	static const char NODEJS_PATH_DELIMITER_CHARACTER = ':';
	static const std::string NODEJS_PATH_DELIMITER_STRING(":");
#endif
        static const char NODEJS_PATH_ESCAPE_CHARACTER = '\\';
        static const std::string NODEJS_PATH_ESCAPE_STRING("\\");

        /**
         * Singleton implementation of a module loader
	 * that knows how to load (and cache) modules
	 * from the filesystem based on a "CLASSPATH" type thing.
	 */
	class ModuleLoaderNodeJSPath : public NodeJS::core::ModuleLoader {
	public:
	    ModuleLoaderNodeJSPath();
	    
	    /**
	     * This overrides the value of the NODEJS_PATH
	     * environment variable for testing and other purposes.
	     * By default, this class loader uses the environment
	     * variable to look for the modules.
	     */
	    void setNODEJS_PATH(std::string path);
	    
	    const std::vector<std::string> & getNODEJS_PATH();

            /**
	     * This loads a module from the NODEJS_PATH
	     * if it exists, or returns nullptr if it does not.
	     *
	     * Errors in loading modules are reported by the error
	     * reporter.
	     */
	    virtual NodeJS::core::NodeModule *loadModule(
		std::string aFullyQualifiedModuleName,
		NodeJS::core::SerializerErrorReporter & reporter
		) override;
	    
	    virtual NodeJS::core::NodeModule *newModule(
		std::string packageName
		) override;
	private:
	    std::map<std::string, std::unique_ptr<NodeJS::core::NodeModule>> mLoadedModules;
	    std::vector<std::string> mPath;
	};
    }
}
