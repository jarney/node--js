#pragma once

#include "node--js/NodeModule.hpp"
#include "node--js/SerializerError.hpp"
#include <istream>
#include <ostream>

namespace NodeJS {
    namespace core {
	class SerializerErrorReporter;
	
        /**
         * This is the base class for serializing node programs
         * to various formats.  This is typically implemented
	 * as a singleton which can be used in a number of
	 * places where a serializer is required to load or save
	 * the content of a module.
         */
	class ModuleLoader {
	public:
	    virtual ~ModuleLoader() = default;

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
	    virtual NodeModule *loadModule(
		std::string aFullyQualifiedModuleName,
		SerializerErrorReporter & reporter
		) = 0;

	    /**
	     * This creates a new (mutable)
	     * module so that new modules can be loaded and
	     * manipulated.  Generally speaking, tools
	     * only ever modify a single module at one time (in theory?)
	     */
	    virtual NodeModule *newModule(
		std::string packageName
		) = 0;
	};

    } // End core
} // End NodeJS
