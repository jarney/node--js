#pragma once

#include "node--js/SerializerError.hpp"
#include "node--js/NodeModule.hpp"

#include <libxml/parser.h>
#include <libxml/tree.h>

namespace NodeJS {
    namespace xml {
	class SerializerXMLReader {
	public:
	    SerializerXMLReader(
		std::string aFilename,
		xmlDocPtr aDocument,
		NodeJS::core::NodeModule & aNodeModule,
		NodeJS::core::SerializerErrorReporter & aErrorReporter
		);
	    
	    ~SerializerXMLReader() = default;
	    bool read(void) const;
	private:
	    std::string mFilename;
	    xmlDocPtr mDocument;
	    NodeJS::core::NodeModule & mNodeModule;
	    NodeJS::core::SerializerErrorReporter & mErrorReporter;
	};
    }
}
