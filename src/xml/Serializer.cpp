#include "node--js/xml/Serializer.hpp"
#include <istream>
#include <libxml/parser.h>
#include <libxml/tree.h>

using namespace NodeJS::xml;

bool
Serializer::write(const NodeJS::core::NodeModule & node_module, std::ostream & output_stream, std::ostream & err) const
{

    xmlDocPtr doc;
    doc = xmlNewDoc(BAD_CAST "1.0");
    xmlNodePtr n;
    n = xmlNewNode(nullptr, BAD_CAST "root");
    xmlNodeSetContent(n, BAD_CAST "content");
    xmlDocSetRootElement(doc, n);
    
    xmlChar *output_mem = nullptr;
    int output_size = 0;
    
    xmlDocDumpFormatMemory(
 	doc, &output_mem, &output_size, 0);

    output_stream.write((const char*)output_mem, output_size);
    
    xmlFreeDoc(doc);
    
    return true;
}

bool
Serializer::read(NodeJS::core::NodeModule & node_module, std::istream & input_stream, std::ostream & err) const
{

    xmlDocPtr doc; /* the resulting document tree */

    std::string json_string(std::istreambuf_iterator<char>(input_stream), {});

    doc = xmlReadMemory(json_string.c_str(), json_string.size(), nullptr, "utf-8", 0);
    if (doc == nullptr) {
        err << "Failed to parse xml document" << std::endl;
	return false;
    }
    
    xmlFreeDoc(doc);
    return true;
}
