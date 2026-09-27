#include "node--js/xml/Serializer.hpp"
#include "node--js/xml/XmlNodeWrapper.hpp"
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
readDataType(NodeJS::core::NodeModule & node_module, XmlNodeWrapper node)
{
    std::string id = node.getAttribute("id");
    std::string name = node.getAttribute("name");
    std::unique_ptr<NodeJS::core::DataType> dataType = std::make_unique<NodeJS::core::DataType>(id, name);
    node_module.addDataType(std::move(dataType));
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
readDataTypes(NodeJS::core::NodeModule & node_module, XmlNodeWrapper dataTypesNode)
{
    for (XmlNodeWrapper::Iterator it = dataTypesNode.begin(); it != dataTypesNode.end(); ++it) {
	XmlNodeWrapper child = it.get();
	if (child.getName() != std::string("data-type")) {
	    continue;
	}
	readDataType(node_module, child);
    }
}

static void
writeDataTypes(const NodeJS::core::NodeModule & node_module, XmlNodeWrapper root)
{
    xmlNodePtr node = xmlNewNode(nullptr, BAD_CAST "data-types");
    XmlNodeWrapper dataTypesNode(node);
    for (const auto & it : node_module.getDataTypes()) {
	writeDataType(it.first, *it.second.get(), dataTypesNode);
    }
    
    root.addChild(dataTypesNode);
}

static void
readPackage(NodeJS::core::NodeModule & node_module, XmlNodeWrapper packageNode)
{
    if (packageNode.hasAttribute("id")) {
	std::string id = packageNode.getAttribute("id");
	node_module.setPackage(id);
    }
}

static void
writePackage(const NodeJS::core::NodeModule & node_module, XmlNodeWrapper root)
{
    xmlNodePtr packageNode = xmlNewNode(nullptr, BAD_CAST "package");
    XmlNodeWrapper package(packageNode);
    package.setAttribute("id", node_module.getPackage());
    root.addChild(packageNode);
}

static void
readNodeTypes(NodeJS::core::NodeModule & node_module, XmlNodeWrapper nodeTypesNode)
{
    for (XmlNodeWrapper::Iterator it = nodeTypesNode.begin(); it != nodeTypesNode.end(); ++it) {
	XmlNodeWrapper child = it.get();
	std::string tagName = child.getName();
	if (tagName != std::string("node-type")) {
	    continue;
	}
	if (!child.hasAttribute("id")) {
	    fprintf(stderr, "Invalid file: no node type id\n");
	    continue;
	}

	NodeJS::core::NodeType::Visibility visibility = NodeJS::core::NodeType::Visibility::PRIVATE;
	if (child.hasAttribute("visibility")) {
	    std::string visibilityStr = child.getAttribute("visibility");
	    if (visibilityStr == std::string("public")) {
		visibility = NodeJS::core::NodeType::Visibility::PUBLIC;
	    }
	}
	NodeJS::core::NodeType::Type type = NodeJS::core::NodeType::Type::GRAPH;
	if (child.hasAttribute("type")) {
	    std::string implStr = child.getAttribute("type");
	    if (implStr == std::string("native")) {
		type = NodeJS::core::NodeType::Type::NATIVE;
	    }
	}
	
	std::string id = child.getAttribute("id");
	std::unique_ptr<NodeJS::core::NodeType> nodeType = std::make_unique<NodeJS::core::NodeType>();
	nodeType->setId(id);
	nodeType->setVisibility(visibility);
	nodeType->setType(type);
	node_module.addNodeType(std::move(nodeType));
    }
}

static void
writeNodeType(const std::string & id, const NodeJS::core::NodeType & node_type, XmlNodeWrapper nodeTypesNode)
{
    xmlNodePtr nodeTypeNode = xmlNewNode(nullptr, BAD_CAST "node-type");
    XmlNodeWrapper nodeType(nodeTypeNode);
    nodeType.setAttribute("id", id);
    if (node_type.getVisibility() == NodeJS::core::NodeType::Visibility::PUBLIC) {
	nodeType.setAttribute("visibility", "public");
    }
    else {
	nodeType.setAttribute("visibility", "private");
    }
    if (node_type.getType() == NodeJS::core::NodeType::Type::GRAPH) {
	nodeType.setAttribute("type", "graph");
    }
    else {
	nodeType.setAttribute("type", "native");
    }
    nodeTypesNode.addChild(nodeTypeNode);
}

static void
writeNodeTypes(const NodeJS::core::NodeModule & node_module, XmlNodeWrapper root)
{
    xmlNodePtr node = xmlNewNode(nullptr, BAD_CAST "node-types");
    XmlNodeWrapper nodeTypesNode(node);
    for (const auto & it : node_module.getNodeTypes()) {
	writeNodeType(it.first, *it.second, nodeTypesNode);
    }
    root.addChild(nodeTypesNode);
}

bool
Serializer::write(const NodeJS::core::NodeModule & node_module, std::ostream & output_stream, std::ostream & err) const
{

    xmlDocPtr doc;
    doc = xmlNewDoc(BAD_CAST "1.0");
    xmlNodePtr root = xmlNewNode(nullptr, BAD_CAST "node-module");
    xmlSetProp(root, BAD_CAST "xmlns", BAD_CAST NODEJS_XML_NAMESPACE);
    xmlDocSetRootElement(doc, root);

    XmlNodeWrapper rootNode(root);
    writePackage(node_module, rootNode);
    writeDataTypes(node_module, rootNode);
    writeNodeTypes(node_module, rootNode);
    
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
    XmlNodeWrapper root(xmlDocGetRootElement(doc));

    // The package needs to be read first because that determines where
    // other data is registered.
    for (XmlNodeWrapper::Iterator it = root.begin(); it != root.end(); ++it) {
	XmlNodeWrapper child = it.get();
	std::string tagName = child.getName();
	if (tagName == std::string("package")) {
	    readPackage(node_module, child);
	}
    }

    // Data types need to be read before node types
    // even if they appear later in the file.
    for (XmlNodeWrapper::Iterator it = root.begin(); it != root.end(); ++it) {
	XmlNodeWrapper child = it.get();
	std::string tagName = child.getName();
	if (tagName == std::string("data-types")) {
	    readDataTypes(node_module, child);
	}
    }

    // Finally, we have enough qualified metadata
    // the node types and graphs from the file.
    for (XmlNodeWrapper::Iterator it = root.begin(); it != root.end(); ++it) {
	XmlNodeWrapper child = it.get();
	std::string tagName = child.getName();
	if (tagName == std::string("node-types")) {
	    readNodeTypes(node_module, child);
	}
    }
    
    xmlFreeDoc(doc);
    return true;
}
