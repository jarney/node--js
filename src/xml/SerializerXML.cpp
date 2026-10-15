#include "node--js/xml/SerializerXML.hpp"
#include "node--js/xml/XmlNodeWrapper.hpp"
#include "node--js/ModuleLoader.hpp"
#include "node--js/Metadata.hpp"

#include "SerializerXMLReader.hpp"
#include "SerializerXMLWriter.hpp"

#include <istream>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <cstring>
#include <cstdlib>

#include <vector>

using namespace NodeJS::xml;
using namespace NodeJS::core;

const SerializerXML &
SerializerXML::instance()
{
    static SerializerXML instance;
    return instance;
}

bool
SerializerXML::write(
    std::string aFilename,
    const NodeModule & node_module,
    std::ostream & output_stream,
    NodeJS::core::SerializerErrorReporter & err
    ) const
{
    xmlDocPtr doc;
    doc = xmlNewDoc(BAD_CAST "1.0");
    SerializerXMLWriter writer(
	aFilename,
	doc,
	node_module,
	err
	);

    writer.write();

    xmlChar *output_mem = nullptr;
    int output_size = 0;
    
    xmlDocDumpFormatMemory(
 	doc, &output_mem, &output_size, 1);

    output_stream.write((const char*)output_mem, output_size);

    free(output_mem);
    
    xmlFreeDoc(doc);
    
    return true;
}

bool
SerializerXML::read(
    std::string filename,
    std::istream & input_stream,
    NodeModule & node_module,
    NodeJS::core::SerializerErrorReporter & err
    ) const
{

    xmlDocPtr doc; /* the resulting document tree */

    std::string json_string(std::istreambuf_iterator<char>(input_stream), {});
    
    doc = xmlReadMemory(json_string.c_str(), json_string.size(), nullptr, "utf-8", XML_PARSE_BIG_LINES);
    if (doc == nullptr) {
        err.reportError(SerializerErrorCode::LEGACY_ERROR_XML_PARSE, 0, "Input Stream", "Failed to parse package xml file:" + filename);
	return false;
    }
    
    SerializerXMLReader reader(filename, doc, node_module, err);
    bool result = reader.read();
    
    xmlFreeDoc(doc);

    return result;
}
