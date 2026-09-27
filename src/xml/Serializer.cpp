#include "node--js/xml/Serializer.hpp"
#include <istream>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <cstring>

#include <vector>

using namespace NodeJS::xml;

static const char *NODEJS_XML_NAMESPACE = "http://jarney.github.io/nodejs-schema";

class XmlNodeWrapper;


class XmlNodeWrapper {
public:

    struct Iterator {
	// Iterator tags here...
	using iterator_category = std::forward_iterator_tag;
	using difference_type   = std::ptrdiff_t;
	using value_type        = xmlNode;
	using pointer           = xmlNodePtr;  // or also value_type*
	using reference         = xmlNode&;  // or also value_type&
	
	// Iterator constructors here...
	Iterator(pointer node) : m_node(node) {}
	
	reference operator*() const { return *m_node; }
	pointer operator->() { return m_node; }
	XmlNodeWrapper get();// { return m_node; }
	
	// Prefix increment
	Iterator& operator++() {
	    m_node = xmlNextElementSibling(m_node);
	    return *this;
	}
	
	// Postfix increment
	Iterator operator++(int) {
	    Iterator tmp = *this;
	    ++(*this);
	    return tmp;
	}
	
	friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_node == b.m_node; };
	friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_node != b.m_node; };     
	
    private:
	pointer m_node;
    };

    XmlNodeWrapper(xmlNodePtr node);
    ~XmlNodeWrapper() = default;
    std::string getName(void) const;
    std::string getAttribute(const std::string & name) const;
    bool hasAttribute(const std::string & name) const;
    void setAttribute(const std::string & name, const std::string & value);
    void addChild(const XmlNodeWrapper & other);
    
    Iterator begin();
    Iterator end();
private:
    xmlNodePtr _node;
};

XmlNodeWrapper
XmlNodeWrapper::Iterator::get() {
    return XmlNodeWrapper(m_node);
}

XmlNodeWrapper::Iterator
XmlNodeWrapper::begin()
{
    return Iterator(xmlFirstElementChild(_node));
}
XmlNodeWrapper::Iterator
XmlNodeWrapper::end()
{
    return Iterator(nullptr);
}

XmlNodeWrapper::XmlNodeWrapper(xmlNodePtr node)
    : _node(node)
{}

std::string
XmlNodeWrapper::getName(void) const
{
    return std::string((const char *)_node->name);
}

std::string
XmlNodeWrapper::getAttribute(const std::string & name) const
{
    const xmlChar *id = xmlGetProp(_node, BAD_CAST name.c_str());
    std::string str((const char *)id);
    xmlFree((void*)id);
    return str;
}
bool
XmlNodeWrapper::hasAttribute(const std::string & name) const
{
    const xmlAttr *attr = xmlHasProp(_node, BAD_CAST name.c_str());
    return attr != nullptr;
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
	writeDataType(it.first, it.second, dataTypesNode);
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
	std::string id = child.getAttribute("id");
	std::unique_ptr<NodeJS::core::NodeType> nodeType = std::make_unique<NodeJS::core::NodeType>();
	node_module.addNodeType(id, std::move(nodeType));
    }
}

static void
writeNodeType(const std::string & id, const NodeJS::core::NodeType & node_type, XmlNodeWrapper nodeTypesNode)
{
    xmlNodePtr nodeTypeNode = xmlNewNode(nullptr, BAD_CAST "node-types");
    XmlNodeWrapper nodeType(nodeTypeNode);
    nodeType.setAttribute("id", id);
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
