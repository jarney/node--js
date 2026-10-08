#include "node--js/xml/SerializerXML.hpp"
#include "node--js/xml/XmlNodeWrapper.hpp"
#include "node--js/ModuleLoader.hpp"
#include "node--js/Metadata.hpp"

#include <istream>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <cstring>
#include <cstdlib>

#include <vector>

using namespace NodeJS::xml;

using NodeJS::core::Node;
using NodeJS::core::EdgeId;
using NodeJS::core::NodePort;
using NodeJS::core::NodeType;
using NodeJS::core::NamedPorts;
using NodeJS::core::NodeModule;
using NodeJS::core::DataType;
using NodeJS::core::NodeGraph;
using NodeJS::core::ConnectionData;
using NodeJS::core::ModuleLoader;
using NodeJS::core::Metadata;

static const char *NODEJS_XML_NAMESPACE = "http://jarney.github.io/nodejs/schema";

static void
writeGraphNodeData(
    const ConnectionData & data,
    XmlNodeWrapper nodeNode,
    NodeJS::core::SerializerErrorReporter & err    
    );
static void
readGraphNodeData(
    ConnectionData & node_data,
    XmlNodeWrapper nodeNode,
    NodeJS::core::SerializerErrorReporter & err
    );

static void
writeMetadata(
    const Metadata & metadata,
    XmlNodeWrapper & parentNode,
    NodeJS::core::SerializerErrorReporter & err
    )
{
    XmlNodeWrapper metadataNode("metadata");
    size_t count = 0;
    for (const auto & ns : metadata.getNamespaces()) {
	XmlNodeWrapper data("data");
	data.setAttribute("namespace", ns);
	writeGraphNodeData(metadata.getMetadata(ns),
			   data,
			   err);
	metadataNode.addChild(data);
	count++;
    }
    if (count > 0) {
	parentNode.addChild(metadataNode);
    }
}

void
readMetadata(
    Metadata & metadata,
    XmlNodeWrapper & parentNode,
    NodeJS::core::SerializerErrorReporter & err
    )
{
    for (XmlNodeWrapper::Iterator it = parentNode.begin(); it != parentNode.end(); ++it) {
	XmlNodeWrapper child = it.get();
	std::string childName = child.getName();
	if (childName == "data") {
	    std::string ns = child.getAttribute("namespace");
	    readGraphNodeData(
		metadata.getMetadata(ns),
		child, err);
	}
    }
}

const SerializerXML &
SerializerXML::instance()
{
    static SerializerXML instance;
    return instance;
}

static void
readDataType(
    NodeModule & node_module,
    XmlNodeWrapper node,
    NodeJS::core::SerializerErrorReporter & err
    )
{
    std::string id = node.getAttribute("id");
    std::string name = node.getAttribute("name");
    std::unique_ptr<DataType> dataType = std::make_unique<DataType>(id, name);
    node_module.addDataType(std::move(dataType));
}

static void
writeDataType(
    const std::string & id,
    const DataType & data_type, XmlNodeWrapper dataTypesNode,
    NodeJS::core::SerializerErrorReporter & err
    )
{
    XmlNodeWrapper node("data-type");
    node.setAttribute("id", id);
    node.setAttribute("name", data_type.getName());
    dataTypesNode.addChild(node);
}

static void
readDataTypes(
    NodeModule & node_module,
    XmlNodeWrapper dataTypesNode,
    NodeJS::core::SerializerErrorReporter & err
    )
{
    for (XmlNodeWrapper::Iterator it = dataTypesNode.begin(); it != dataTypesNode.end(); ++it) {
	XmlNodeWrapper child = it.get();
	if (child.getName() != std::string("data-type")) {
	    continue;
	}
	readDataType(node_module, child, err);
    }
}

static void
writeDataTypes(
    const NodeModule & node_module,
    XmlNodeWrapper root,
    NodeJS::core::SerializerErrorReporter & err
    )
{
    XmlNodeWrapper dataTypesNode("data-types");
    for (const auto & it : node_module.getDataTypes()) {
	writeDataType(it.first, *it.second.get(), dataTypesNode, err);
    }
    
    root.addChild(dataTypesNode);
}

static void
readPackage(
    NodeModule & node_module,
    XmlNodeWrapper packageNode,
    NodeJS::core::SerializerErrorReporter & err
    )    
{
    if (packageNode.hasAttribute("id")) {
	std::string id = packageNode.getAttribute("id");
	node_module.setPackage(id);
    }
    if (packageNode.hasAttribute("description")) {
	std::string desc = packageNode.getAttribute("description");
	node_module.setDescription(desc);
    }
}

static void
writePackage(
    const NodeModule & node_module,
    XmlNodeWrapper root,
    NodeJS::core::SerializerErrorReporter & err
    )
{
    XmlNodeWrapper package("package");
    package.setAttribute("id", node_module.getPackage());
    package.setAttribute("description", node_module.getDescription());
    root.addChild(package);
}

static void
readNamedPorts(
    NamedPorts & namedPorts,
    XmlNodeWrapper inputsNode,
    NodeJS::core::SerializerErrorReporter & err
    )    
{
    for (XmlNodeWrapper::Iterator it = inputsNode.begin(); it != inputsNode.end(); ++it) {
	XmlNodeWrapper child = it.get();
	std::string childName = child.getName();
	if (childName != "port") continue;

	std::string id = child.getAttribute("id");
	std::string description = child.getAttribute("description");
	std::string dataType = child.getAttribute("data-type");
	NodePort::ConnectionPolicy connectionPolicy = NodePort::ConnectionPolicy::One;
	if (child.hasAttribute("connection-policy")) {
	    std::string connectionPolicyStr = child.getAttribute("connection-policy");
	    if (connectionPolicyStr == "multi") {
		connectionPolicy = NodePort::ConnectionPolicy::Multiple;
	    }
	}
	std::unique_ptr<NodePort> port = std::make_unique<NodePort>(dataType, description, connectionPolicy);
    
	for (XmlNodeWrapper::Iterator itPortChildren = child.begin(); itPortChildren != child.end(); ++itPortChildren) {
	    XmlNodeWrapper portChild = itPortChildren.get();
	    std::string portChildName = portChild.getName();
	    if (portChildName == "metadata") {
		readMetadata(port->getMetadata(), portChild, err);
	    }
	}
	
	namedPorts.addPort(id, std::move(port));
    }
}

static void
readNodeType(
    NodeModule & node_module,
    XmlNodeWrapper nodeTypeNode,
    NodeJS::core::SerializerErrorReporter & err
    )
{
    NodeType::Visibility visibility = NodeType::Visibility::PRIVATE;
    if (nodeTypeNode.hasAttribute("visibility")) {
	std::string visibilityStr = nodeTypeNode.getAttribute("visibility");
	if (visibilityStr == std::string("public")) {
	    visibility = NodeType::Visibility::PUBLIC;
	}
    }
    NodeType::Type type = NodeType::Type::GRAPH;
    if (nodeTypeNode.hasAttribute("type")) {
	std::string implStr = nodeTypeNode.getAttribute("type");
	if (implStr == std::string("native")) {
	    type = NodeType::Type::NATIVE;
	}
    }

    std::string id = nodeTypeNode.getAttribute("id");
    std::unique_ptr<NodeType> nodeType = std::make_unique<NodeType>();
    nodeType->setId(id);
    nodeType->setVisibility(visibility);
    nodeType->setType(type);

    if (nodeTypeNode.hasAttribute("allow-port-override")) {
	bool allowOverride = nodeTypeNode.getAttribute("allow-port-override") == "true";
	nodeType->setAllowPortOverride(allowOverride);
    }
    
    for (XmlNodeWrapper::Iterator it = nodeTypeNode.begin(); it != nodeTypeNode.end(); ++it) {
	XmlNodeWrapper child = it.get();
	std::string childName = child.getName();
	if (childName == "inputs") {
	    readNamedPorts(nodeType.get()->getInputs(), child, err);
	}
	else if (childName == "outputs") {
	    readNamedPorts(nodeType.get()->getOutputs(), child, err);
	}
	else if (childName == "default-node-data") {
	    ConnectionData node_data;
	    readGraphNodeData(node_data, child, err);
	    nodeType->setDefaultNodeData(node_data);
	}
	else if (childName == std::string("metadata")) {
	    readMetadata(nodeType.get()->getMetadata(), child, err);
	}
	else {
	    // Nothing to do, this is extraneous.
	}
    
    }
    
    node_module.addNodeType(std::move(nodeType));
}

static void
writeNamedPorts(
    const NamedPorts & namedPorts,
    XmlNodeWrapper portsNode,
    NodeJS::core::SerializerErrorReporter & err
    )
{
    for (size_t i = 0; i < namedPorts.getCount(); i++) {
	std::string id = namedPorts.getName(i);
	const NodePort *port = namedPorts.getByIndex(i);
	XmlNodeWrapper portNode("port");
	portNode.setAttribute("id", id);
	portNode.setAttribute("description", port->getDescription());
	portNode.setAttribute("data-type", port->getDataType());
	portNode.setAttribute("connection-policy", port->getConnectionPolicy() == NodePort::ConnectionPolicy::Multiple ? "multi" : "one");

	writeMetadata(port->getMetadata(), portNode, err);
	
	portsNode.addChild(portNode);
    }

}

static void
writeNodeType(
    const std::string & id,
    const NodeType & node_type,
    XmlNodeWrapper nodeTypesNode,
    NodeJS::core::SerializerErrorReporter & err
    )
{
    XmlNodeWrapper nodeType("node-type");
    nodeType.setAttribute("id", id);
    if (node_type.getVisibility() == NodeType::Visibility::PUBLIC) {
	nodeType.setAttribute("visibility", "public");
    }
    else {
	nodeType.setAttribute("visibility", "private");
    }
    if (node_type.getType() == NodeType::Type::GRAPH) {
	nodeType.setAttribute("type", "graph");
    }
    else {
	nodeType.setAttribute("type", "native");
    }

    writeMetadata(node_type.getMetadata(), nodeType, err);

    XmlNodeWrapper inputsNode("inputs");
    writeNamedPorts(node_type.getInputs(), inputsNode, err);
    nodeType.addChild(inputsNode);
    
    XmlNodeWrapper outputsNode("outputs");
    writeNamedPorts(node_type.getOutputs(), outputsNode, err);
    nodeType.addChild(outputsNode);

    const ConnectionData & defaultNodeData = node_type.getDefaultNodeData();
    if (defaultNodeData.size() != 0) {
	XmlNodeWrapper defaultNodeDataNode("default-node-data");
	writeGraphNodeData(defaultNodeData, defaultNodeDataNode, err);
	nodeType.addChild(defaultNodeDataNode);
    }
    
    nodeTypesNode.addChild(nodeType);
}

static void
readNodeTypes(
    NodeModule & node_module,
    XmlNodeWrapper nodeTypesNode,
    NodeJS::core::SerializerErrorReporter & err
    )    
{
    for (XmlNodeWrapper::Iterator it = nodeTypesNode.begin(); it != nodeTypesNode.end(); ++it) {
	XmlNodeWrapper child = it.get();
	std::string childName = child.getName();
	if (childName != std::string("node-type")) {
	    continue;
	}
	if (!child.hasAttribute("id")) {
	    fprintf(stderr, "Invalid file: no node type id\n");
	    continue;
	}
	readNodeType(node_module, child, err);
    }
}

static void
writeNodeTypes(
    const NodeModule & node_module,
    XmlNodeWrapper root,
    NodeJS::core::SerializerErrorReporter & err
    )    
{
    XmlNodeWrapper nodeTypesNode("node-types");
    for (const auto & it : node_module.getNodeTypes()) {
	writeNodeType(it.first, *it.second, nodeTypesNode, err);
    }
    root.addChild(nodeTypesNode);
}

static void
readGraphNodeData(
    ConnectionData & node_data,
    XmlNodeWrapper nodeNode,
    NodeJS::core::SerializerErrorReporter & err
    )
{
    for (XmlNodeWrapper::Iterator it = nodeNode.begin(); it != nodeNode.end(); ++it) {
	XmlNodeWrapper child = it.get();
	std::string childName = child.getName();
	if (childName == std::string("value")) {
	    std::string key = child.getAttribute("key");
	    std::string value = child.getContent();
	    node_data.setValue(key, value);
	}
    }
}

static void
readGraphNode(
    NodeModule & node_module,
    NodeGraph & node_graph,
    XmlNodeWrapper nodeNode,
    NodeJS::core::SerializerErrorReporter & err
    )
{
    std::string nodeTypeName = nodeNode.getAttribute("type");
    const NodeType *nodeType = node_graph.getNodeType(nodeTypeName);
    if (nodeType == nullptr) {
	err.reportError(-99, 22, "context", std::string("Node type ") + nodeTypeName + std::string(" does not exist"));
	return;
    }
    
    std::string nodeId = nodeNode.getAttribute("id");

    ConnectionData node_data;
    readGraphNodeData(node_data, nodeNode, err);

    Node & node = node_graph.newNode(
	*nodeType,
	nodeId,
	node_data
	);

    for (XmlNodeWrapper::Iterator it = nodeNode.begin(); it != nodeNode.end(); ++it) {
	XmlNodeWrapper child = it.get();
	std::string childName = child.getName();
	if (nodeType->getAllowPortOverride()) {
	    if (childName == std::string("inputs")) {
		node.setOverrideInputs(true);
		readNamedPorts(node.getOverrideInputs(), child, err);
	    }
	    if (childName == std::string("outputs")) {
		node.setOverrideOutputs(true);
		readNamedPorts(node.getOverrideOutputs(), child, err);
	    }
	}
	if (childName == std::string("metadata")) {
	    readMetadata(node.getMetadata(), child, err);
	}
    }
}

static void
readGraphNodes(
    NodeModule & node_module,
    NodeGraph & node_graph,
    XmlNodeWrapper nodesNode,
    NodeJS::core::SerializerErrorReporter & err
    )
{
    for (XmlNodeWrapper::Iterator it = nodesNode.begin(); it != nodesNode.end(); ++it) {
	XmlNodeWrapper child = it.get();
	std::string childName = child.getName();
	if (childName == std::string("node")) {
	    readGraphNode(node_module, node_graph, child, err);
	}

    }
}

static void
readGraphEdge(
    NodeModule & node_module,
    NodeGraph & node_graph,
    XmlNodeWrapper edgeNode,
    NodeJS::core::SerializerErrorReporter & err
    )
{
    std::string fromNode = edgeNode.getAttribute("from-node");
    std::string toNode = edgeNode.getAttribute("to-node");
    std::string fromPort = edgeNode.getAttribute("from-port");
    std::string toPort = edgeNode.getAttribute("to-port");

    std::optional<EdgeId> connection = 
	node_graph.newEdge(
	    fromNode, fromPort,
	    toNode, toPort
	    );

    if (!connection.has_value()) {
	err.reportError(SerializerXML::ERROR_XML_PARSE, 0, "Input Stream", "Duplicate graph edge");
	return;
    }
}

static void
readGraphEdges(
    NodeModule & node_module,
    NodeGraph & node_graph,
    XmlNodeWrapper nodesNode,
    NodeJS::core::SerializerErrorReporter & err
    )
{
    for (XmlNodeWrapper::Iterator it = nodesNode.begin(); it != nodesNode.end(); ++it) {
	XmlNodeWrapper child = it.get();
	std::string childName = child.getName();
	if (childName == std::string("edge")) {
	    readGraphEdge(node_module, node_graph, child, err);
	}
    }
}

static void
readGraphScope(
    NodeModule & node_module,
    NodeGraph & node_graph,
    XmlNodeWrapper scopeNode,
    NodeJS::core::SerializerErrorReporter & err
    )
{
    for (XmlNodeWrapper::Iterator it = scopeNode.begin(); it != scopeNode.end(); ++it) {
	XmlNodeWrapper child = it.get();
	std::string childName = child.getName();
	if (childName == std::string("package")) {
	    ModuleLoader & loader = node_module.getModuleLoader();
	    const NodeModule *scope_package = loader.loadModule(child.getAttribute("id"), err);
	    if (scope_package) {
		node_graph.addScope(scope_package);
	    }
	    else {
		fprintf(stderr, "Failed to load scope %s\n", child.getAttribute("id").c_str());
		return;
	    }
	}
    }
}

static void
readGraph(
    NodeModule & node_module,
    XmlNodeWrapper graphNode,
    NodeJS::core::SerializerErrorReporter & err
    )
{
    std::string id = graphNode.getAttribute("id");
    NodeGraph *graph = node_module.addGraph(id);

    // Scopes must be read first because
    // we may need to resolve node types
    // using the scope if they are externally
    // defined.  How will we deal with this
    // if we need to recursively load?
    for (XmlNodeWrapper::Iterator it = graphNode.begin(); it != graphNode.end(); ++it) {
	XmlNodeWrapper child = it.get();
	std::string childName = child.getName();
	if (childName == std::string("scope")) {
	    readGraphScope(node_module, *graph, child, err);
	}
	else if (childName == std::string("metadata")) {
	    readMetadata(graph->getMetadata(), child, err);
	}
    }
    
    // All nodes must be read before any edges
    for (XmlNodeWrapper::Iterator it = graphNode.begin(); it != graphNode.end(); ++it) {
	XmlNodeWrapper child = it.get();
	std::string childName = child.getName();
	if (childName == std::string("nodes")) {
	    readGraphNodes(node_module, *graph, child, err);
	}
    }
    for (XmlNodeWrapper::Iterator it = graphNode.begin(); it != graphNode.end(); ++it) {
	XmlNodeWrapper child = it.get();
	std::string childName = child.getName();
	if (childName == std::string("edges")) {
	    readGraphEdges(node_module, *graph, child, err);
	}
    }    
}

static void
readGraphs(
    NodeModule & node_module,
    XmlNodeWrapper nodeTypesNode,
    NodeJS::core::SerializerErrorReporter & err
    )
{
    for (XmlNodeWrapper::Iterator it = nodeTypesNode.begin(); it != nodeTypesNode.end(); ++it) {
	XmlNodeWrapper child = it.get();
	std::string graphName = child.getName();
	if (graphName != std::string("graph")) {
	    continue;
	}
	if (!child.hasAttribute("id")) {
	    err.reportError(SerializerXML::ERROR_XML_PARSE, 0, "Input Stream", "Invalid file, graph without id");
	    continue;
	}
	readGraph(node_module, child, err);
    }
}

static void
writeGraphNodeData(
    const ConnectionData & data,
    XmlNodeWrapper nodeNode,
    NodeJS::core::SerializerErrorReporter & err    
    )
{
    for (const auto & it : data.getData()) {
	XmlNodeWrapper valueNode("value");

	valueNode.setAttribute("key", it.first);
	valueNode.setContent(it.second);
	
	nodeNode.addChild(valueNode);
    }
}

static void
writeGraphNode(
    const Node & node,
    XmlNodeWrapper nodeNode,
    NodeJS::core::SerializerErrorReporter & err
    )
{
    const NodeType & type = node.getType();
    nodeNode.setAttribute("type", type.getId());
    writeGraphNodeData(node.getData(), nodeNode, err);
    
    if (type.getAllowPortOverride()) {
	if (node.hasOverrideInputs()) {
	    XmlNodeWrapper inputsNode("inputs");
	    writeNamedPorts(node.getOverrideInputs(), inputsNode, err);
	    nodeNode.addChild(inputsNode);
	}
	
	if (node.hasOverrideOutputs()) {
	    XmlNodeWrapper outputsNode("outputs");
	    writeNamedPorts(node.getOverrideOutputs(), outputsNode, err);
	    nodeNode.addChild(outputsNode);
	}
    }
    
    writeMetadata(node.getMetadata(), nodeNode, err);
}


static void
writeGraphNodes(
    const NodeGraph & graph,
    XmlNodeWrapper nodesNode,
    NodeJS::core::SerializerErrorReporter & err
    )
{
    for (const auto & it : graph.getNodes()) {
	XmlNodeWrapper nodeNode("node");
	const Node &node = *it.second.get();

	nodeNode.setAttribute("id", it.first);
	writeGraphNode(node, nodeNode, err);
	
	nodesNode.addChild(nodeNode);
    }
    
    
}
    
static void
writeGraphEdges(
    const NodeGraph & graph,
    XmlNodeWrapper edgesNode,
    NodeJS::core::SerializerErrorReporter & err
    )
{
    for (const auto & it : graph.getEdges()) {
	XmlNodeWrapper edgeNode("edge");

	edgeNode.setAttribute("from-node", it.second->fromNode);
	edgeNode.setAttribute("from-port", it.second->fromPort);
	edgeNode.setAttribute("to-node", it.second->toNode);
	edgeNode.setAttribute("to-port", it.second->toPort);
	
	edgesNode.addChild(edgeNode);
    }
}

static void
writeGraphScopeNodes(
    const NodeGraph & graph,
    XmlNodeWrapper scopesNode,
    NodeJS::core::SerializerErrorReporter & err
    )
{
    // Skip the first scope because that's
    // always the 'current' scope, so we don't want
    // to include it artificially
    int i = 0;
    for (const auto & module : graph.getScopes()) {
	if (i != 0) {
	    XmlNodeWrapper packageNode("package");
	    packageNode.setAttribute("id", module->getPackage());
	    scopesNode.addChild(packageNode);
	}
	i++;
    }
}
    

static void
writeGraph(
    std::string id,
    const NodeGraph & graph,
    XmlNodeWrapper graphsNode,
    NodeJS::core::SerializerErrorReporter & err
    )
{
    XmlNodeWrapper graphNode("graph");
    graphNode.setAttribute("id", id);

    writeMetadata(graph.getMetadata(), graphNode, err);
    
    XmlNodeWrapper scopeNode("scope");
    writeGraphScopeNodes(graph, scopeNode, err);
    graphNode.addChild(scopeNode);

    XmlNodeWrapper nodesNode("nodes");
    writeGraphNodes(graph, nodesNode, err);
    graphNode.addChild(nodesNode);

    XmlNodeWrapper edgesNode("edges");
    writeGraphEdges(graph, edgesNode, err);
    graphNode.addChild(edgesNode);

    graphsNode.addChild(graphNode);
}

static void
writeGraphs(
    const NodeModule & node_module,
    XmlNodeWrapper root,
    NodeJS::core::SerializerErrorReporter & err
    )
{
    XmlNodeWrapper graphsNode("graphs");
    for (const auto & it : node_module.getGraphs()) {
	writeGraph(it.first, *it.second, graphsNode, err);
    }
    root.addChild(graphsNode);
}


bool
SerializerXML::write(
    const NodeModule & node_module,
    std::ostream & output_stream,
    NodeJS::core::SerializerErrorReporter & err
    ) const
{
    xmlDocPtr doc;
    doc = xmlNewDoc(BAD_CAST "1.0");
    XmlNodeWrapper rootNode("node-module");
    rootNode.setAttribute("xmlns", NODEJS_XML_NAMESPACE);

    writePackage(node_module, rootNode, err);
    writeMetadata(node_module.getMetadata(), rootNode, err);
    writeDataTypes(node_module, rootNode, err);
    writeNodeTypes(node_module, rootNode, err);
    writeGraphs(node_module, rootNode, err);
    
    xmlDocSetRootElement(doc, rootNode.releasePointer());

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
    NodeModule & node_module,
    std::istream & input_stream,
    NodeJS::core::SerializerErrorReporter & err
    ) const
{
    xmlDocPtr doc; /* the resulting document tree */

    std::string json_string(std::istreambuf_iterator<char>(input_stream), {});
    
    doc = xmlReadMemory(json_string.c_str(), json_string.size(), nullptr, "utf-8", XML_PARSE_BIG_LINES);
    if (doc == nullptr) {
        err.reportError(SerializerXML::ERROR_XML_PARSE, 0, "Input Stream", "Failed to parse xml document");
	return false;
    }

    /*Get the root element node */
    XmlNodeWrapper root(xmlDocGetRootElement(doc), false);

    // The package needs to be read first because that determines where
    // other data is registered.
    for (XmlNodeWrapper::Iterator it = root.begin(); it != root.end(); ++it) {
	XmlNodeWrapper child = it.get();
	std::string childName = child.getName();
	if (childName == std::string("package")) {
	    readPackage(node_module, child, err);
	}
    }

    // Data types need to be read before node types
    // even if they appear later in the file.
    for (XmlNodeWrapper::Iterator it = root.begin(); it != root.end(); ++it) {
	XmlNodeWrapper child = it.get();
	std::string childName = child.getName();
	if (childName == std::string("data-types")) {
	    readDataTypes(node_module, child, err);
	}
	else if (childName == std::string("metadata")) {
	    readMetadata(node_module.getMetadata(), child, err);
	}
    }
    
    // Node types must be loaded before we load any nodes
    for (XmlNodeWrapper::Iterator it = root.begin(); it != root.end(); ++it) {
	XmlNodeWrapper child = it.get();
	std::string childName = child.getName();
	if (childName == std::string("node-types")) {
	    readNodeTypes(node_module, child, err);
	}
    }

    // Finally, we have enough qualified metadata
    // the node types and graphs from the file.
    for (XmlNodeWrapper::Iterator it = root.begin(); it != root.end(); ++it) {
	XmlNodeWrapper child = it.get();
	std::string childName = child.getName();
	if (childName == std::string("graphs")) {
	    readGraphs(node_module, child, err);
	}
    }
    
    xmlFreeDoc(doc);
    return true;
}
