#include "node--js/xml/Serializer.hpp"
#include <istream>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <cstring>

#include <vector>

using namespace NodeJS::xml;

static const char *NODEJS_XML_NAMESPACE = "http://jarney.github.io/nodejs-schema";

class XmlNodeWrapper {
public:
    XmlNodeWrapper(xmlNodePtr node);
    ~XmlNodeWrapper() = default;
    std::string getAttribute(const std::string & name) const;
    void setAttribute(const std::string & name, const std::string & value);
    void addChild(const XmlNodeWrapper & other);
private:
    xmlNodePtr _node;
};

XmlNodeWrapper::XmlNodeWrapper(xmlNodePtr node)
    : _node(node)
{}

std::string
XmlNodeWrapper::getAttribute(const std::string & name) const
{
    const xmlChar *id = xmlGetProp(_node, BAD_CAST name.c_str());
    std::string str((const char *)id);
    xmlFree((void*)id);
    return str;
}
void
XmlNodeWrapper::setAttribute(const std::string & name, const std::string & value)
{
    xmlSetProp(_node, BAD_CAST name.c_str(), BAD_CAST value.c_str());
}

void
XmlNodeWrapper::addChild(const XmlNodeWrapper & other)
{
    xmlAddChild(_node, other._node);
}


const Serializer &
Serializer::instance()
{
    static Serializer instance;
    return instance;
}

static void
readDataType(NodeJS::core::NodeModule & node_module, XmlNodeWrapper node)
{
    std::string id = node.getAttribute("id");
    std::string name = node.getAttribute("name");
    NodeJS::core::DataType dataType(id, name);
    node_module.addDataType(dataType);
}

static void
writeDataType(const std::string & id, const NodeJS::core::DataType & data_type, XmlNodeWrapper dataTypesNode)
{
    xmlNodePtr dataTypeNode = xmlNewNode(nullptr, BAD_CAST "data-type");
    XmlNodeWrapper node(dataTypeNode);
    node.setAttribute("id", id);
    node.setAttribute("name", data_type.getName());
    dataTypesNode.addChild(dataTypeNode);
}

static void
readDataTypes(NodeJS::core::NodeModule & node_module, xmlNodePtr dataTypesNode)
{
    xmlNodePtr child = xmlFirstElementChild(dataTypesNode);
    while (child != nullptr) {
	if (!strcmp((const char *)child->name, "data-type")) {
	    readDataType(node_module, XmlNodeWrapper(child));
	}
	child = xmlNextElementSibling(child);
    }
}

static void
writeDataTypes(const NodeJS::core::NodeModule & node_module, XmlNodeWrapper root)
{
    xmlNodePtr node = xmlNewNode(nullptr, BAD_CAST "data-types");
    XmlNodeWrapper dataTypesNode(node);
    std::vector<std::string> strings{"hello", "world"};
    for (const auto & it : node_module.getDataTypes()) {
	writeDataType(it.first, it.second, dataTypesNode);
    }
    
    root.addChild(dataTypesNode);
}

bool
Serializer::write(const NodeJS::core::NodeModule & node_module, std::ostream & output_stream, std::ostream & err) const
{

    xmlDocPtr doc;
    doc = xmlNewDoc(BAD_CAST "1.0");
    xmlNodePtr root = xmlNewNode(nullptr, BAD_CAST "node-module");
    xmlSetProp(root, BAD_CAST "xmlns", BAD_CAST NODEJS_XML_NAMESPACE);
    xmlDocSetRootElement(doc, root);

    writeDataTypes(node_module, XmlNodeWrapper(root));
    
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
