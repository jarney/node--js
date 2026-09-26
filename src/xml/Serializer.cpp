#include "node--js/xml/Serializer.hpp"
#include <istream>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <cstring>

#include <vector>

using namespace NodeJS::xml;

static const char *NODEJS_XML_NAMESPACE = "http://jarney.github.io/nodejs-schema";

const Serializer &
Serializer::instance()
{
    static Serializer instance;
    return instance;
}

static void
readDataType(NodeJS::core::NodeModule & node_module, xmlNodePtr node)
{
    const xmlChar *name = xmlGetProp(node, BAD_CAST "name");
    const xmlChar *id = xmlGetProp(node, BAD_CAST "id");
    NodeJS::core::DataType dataType(
	std::string((const char*) id),
	std::string((const char*) name)
	);
    free((void*)name);
    free((void*)id);
    node_module.addDataType(dataType);
}

static void
writeDataType(const std::string & id, const NodeJS::core::DataType & data_type, xmlNodePtr dataTypesNode)
{
    xmlNodePtr dataTypeNode = xmlNewNode(nullptr, BAD_CAST "data-type");
    xmlSetProp(dataTypeNode, BAD_CAST "id", BAD_CAST id.c_str());
    xmlSetProp(dataTypeNode, BAD_CAST "name", BAD_CAST data_type.getName().c_str());
    xmlAddChild(dataTypesNode, dataTypeNode);
}

static void
readDataTypes(NodeJS::core::NodeModule & node_module, xmlNodePtr dataTypesNode)
{
    xmlNodePtr child = xmlFirstElementChild(dataTypesNode);
    while (child != nullptr) {
	if (!strcmp((const char *)child->name, "data-type")) {
	    readDataType(node_module, child);
	}
	child = xmlNextElementSibling(child);
    }
}

static void
writeDataTypes(const NodeJS::core::NodeModule & node_module, xmlNodePtr root)
{
    xmlNodePtr dataTypesNode = xmlNewNode(nullptr, BAD_CAST "data-types");

    std::vector<std::string> strings{"hello", "world"};
    for (const auto & it : node_module.getDataTypes()) {
	writeDataType(it.first, it.second, dataTypesNode);
    }
    
    xmlAddChild(root, dataTypesNode);
}

bool
Serializer::write(const NodeJS::core::NodeModule & node_module, std::ostream & output_stream, std::ostream & err) const
{

    xmlDocPtr doc;
    doc = xmlNewDoc(BAD_CAST "1.0");
    xmlNodePtr root = xmlNewNode(nullptr, BAD_CAST "node-module");
    xmlSetProp(root, BAD_CAST "xmlns", BAD_CAST NODEJS_XML_NAMESPACE);
    xmlDocSetRootElement(doc, root);

    writeDataTypes(node_module, root);
    
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
Serializer::read(NodeJS::core::NodeModule & node_module, std::istream & input_stream, std::ostream & err) const
{

    xmlDocPtr doc; /* the resulting document tree */

    std::string json_string(std::istreambuf_iterator<char>(input_stream), {});

    doc = xmlReadMemory(json_string.c_str(), json_string.size(), nullptr, "utf-8", 0);
    if (doc == nullptr) {
        err << "Failed to parse xml document" << std::endl;
	return false;
    }

    /*Get the root element node */
    xmlNodePtr root = xmlDocGetRootElement(doc);
    
    xmlNodePtr dataTypesChild = xmlFirstElementChild(root);
    while (dataTypesChild != nullptr) {
	if (!strcmp((const char *)dataTypesChild->name, "data-types")) {
	    readDataTypes(node_module, dataTypesChild);
	}
	dataTypesChild = xmlNextElementSibling(dataTypesChild);
    }

    
    xmlFreeDoc(doc);
    return true;
}
