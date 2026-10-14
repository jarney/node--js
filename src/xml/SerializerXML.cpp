#include "node--js/xml/SerializerXML.hpp"
#include "node--js/xml/XmlNodeWrapper.hpp"
#include "node--js/ModuleLoader.hpp"
#include "node--js/Metadata.hpp"

#include "SerializerXMLReader.hpp"

#include <istream>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <cstring>
#include <cstdlib>

#include <vector>

using namespace NodeJS::xml;
using namespace NodeJS::core;
#if 0
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
using NodeJS::core::SerializerErrorCode;
using NodeJS::core::SerializerErrorReporterByCode;
#endif

static const char *NODEJS_XML_NAMESPACE = "http://jarney.github.io/nodejs/schema";

static void
writeGraphNodeData(
    const ConnectionData & data,
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

const SerializerXML &
SerializerXML::instance()
{
    static SerializerXML instance;
    return instance;
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
