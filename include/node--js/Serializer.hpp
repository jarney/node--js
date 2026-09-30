#pragma once

#include "node--js/NodeModule.hpp"
#include "node--js/SerializerError.hpp"
#include <istream>
#include <ostream>

namespace NodeJS {
    namespace core {


	class SerializerSerializerErrorReporter;
	
        /**
         * This is the base class for serializing node programs
         * to various formats.  This is typically implemented
	 * as a singleton which can be used in a number of
	 * places where a serializer is required to load or save
	 * the content of a module.
         */
	class Serializer {
	public:
	    virtual ~Serializer() = default;

	    /**
	     * This method writes a node module to the given output
	     * stream.  It will return true if the method successfully
	     * wrote all of the content and false if it did not.  It will
	     * report any errors during serialization to the error_stream.
	     * Typically, these errors would arise from I/O errors in the
	     * underlying output stream given for writing.  Errors encountered
	     * during error writing may not be reported.
	     */
	    virtual bool write(
		const NodeJS::core::NodeModule &node_module,
		std::ostream & output_stream,
		SerializerErrorReporter & error_reporter
		) const = 0;

	    /**
	     * This method reads a node module from the given input
	     * source.  It will return true if a valid node program could
	     * be read from that source and false if not.  Errors will
	     * be reported on the error stream.  Errors may arise from
	     * the underlying input stream or may arise from invalid
	     * content in the stream such as semantic errors in the
	     * node graph or validation errors of the content.
	     */
	    virtual bool read(
		NodeJS::core::NodeModule & node_module,
		std::istream & input_stream,
		SerializerErrorReporter & error_reporter
		) const = 0;
	};

    } // End core
} // End NodeJS
