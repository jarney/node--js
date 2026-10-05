#pragma once

#include "node--js/Serializer.hpp"

namespace NodeJS {
    namespace xml {

/**
 * Singleton implementation of a serializer
 * that serializes the node program to and
 * from JSON.
 */
	class SerializerXML : public NodeJS::core::Serializer {
	private:
	    /**
	     * Constructor is private because we are a singleton.
	     */
	    SerializerXML() = default;
	    ~SerializerXML() = default;
	public:
	    /**
	     * No copy because we are a singleton.
	     */
	    SerializerXML(SerializerXML const &other) = delete;
	    /**
	     * No copy because we are a singleton.
	     */
	    void operator=(SerializerXML const &other)  = delete;
	    
	    /**
	     * Public method to return the serializer.
	     */
	    static const SerializerXML & instance();
	    /**
	     * Writes the given node program to
	     * the given stream using the serialization
	     * method of JSON output.
	     */
	    bool write(
		const NodeJS::core::NodeModule &program,
		std::ostream & output_stream,
		NodeJS::core::SerializerErrorReporter & error_reporter
		) const override;
	    /**
	     * Reads the input stream and fills in the (assumed empty)
	     * node program based on the file content.
	     */
	    bool read(
		NodeJS::core::NodeModule & program,
		std::istream & input_stream,
		NodeJS::core::SerializerErrorReporter & error_reporter
		) const override;

	    static const unsigned long ERROR_XML_PARSE = 1;
	    
	};
    }
}
