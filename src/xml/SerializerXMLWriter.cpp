#include "node--js/ModuleLoader.hpp"
#include "node--js/QualifiedIdentifier.hpp"
#include "node--js/Metadata.hpp"

#include "node--js/xml/SerializerXML.hpp"
#include "SerializerXMLWriter.hpp"

using namespace NodeJS::xml;
using namespace NodeJS::core;

SerializerXMLWriter::SerializerXMLWriter(
    std::string aFilename,
    xmlDocPtr aDocument,
    const NodeModule & aNodeModule,
    SerializerErrorReporter & aErrorReporter
    )
    : mFilename(aFilename)
    , mDocument(aDocument)
    , mNodeModule(aNodeModule)
    , mReporter(aErrorReporter)
    , mRoot("node-module")
{}


void
SerializerXMLWriter::writePackage(void)
{
    XmlNodeWrapper packageEl("package");
    packageEl.setAttribute("id", mNodeModule.getPackage());
    packageEl.setAttribute("version", "1");
    packageEl.setAttribute("description", mNodeModule.getDescription());
    mRoot.addChild(packageEl);

    writeMetadata(mNodeModule.getMetadata(), mRoot);
}

void
SerializerXMLWriter::writeScopes(void)
{
    XmlNodeWrapper scopeNode("scope");
    // Skip the first scope because that's
    // always the 'current' scope, so we don't want
    // to include it artificially
    for (const auto & packageNamespace : mNodeModule.getImportModuleList()) {
	XmlNodeWrapper packageNode("package");
	packageNode.setAttribute("id", packageNamespace);
	scopeNode.addChild(packageNode);
    }
    mRoot.addChild(scopeNode);
}

void
SerializerXMLWriter::writeMetadata(
    const Metadata & metadata,
    XmlNodeWrapper & parentNode
    )
{
    XmlNodeWrapper metadataNode("metadata");
    size_t count = 0;
    for (const auto & ns : metadata.getNamespaces()) {
	XmlNodeWrapper data("data");
	data.setAttribute("namespace", ns);
	writeGraphNodeData(metadata.getMetadata(ns),
			   data);
	metadataNode.addChild(data);
	count++;
    }
    if (count > 0) {
	parentNode.addChild(metadataNode);
    }
}

void
SerializerXMLWriter::writeDataType(
    const std::string & id,
    const DataType & data_type, XmlNodeWrapper dataTypesNode
    )
{
    XmlNodeWrapper node("data-type");
    node.setAttribute("id", data_type.getLocalId());
    node.setAttribute("name", data_type.getName());
    dataTypesNode.addChild(node);
}

void
SerializerXMLWriter::writeDataTypes(void)
{
    XmlNodeWrapper dataTypesNode("data-types");
    for (const auto & it : mNodeModule.getDataTypes()) {
	writeDataType(it.first, *it.second.get(), dataTypesNode);
    }
    
    mRoot.addChild(dataTypesNode);
}

void
SerializerXMLWriter::writeNamedPorts(
    const NamedPorts & namedPorts,
    XmlNodeWrapper portsNode
    )
{
    for (size_t i = 0; i < namedPorts.getCount(); i++) {
	std::string id = namedPorts.getName(i);
	const NodePort *port = namedPorts.getByIndex(i);
	XmlNodeWrapper portNode("port");
	portNode.setAttribute("id", id);
	portNode.setAttribute("description", port->getDescription());
	
        // Un-qualify this if it's a part of our own package,
	QualifiedIdentifier qualifiedDataType(port->getDataType());
	std::string writtenDataType;
	if (qualifiedDataType.getNamespace() == mNodeModule.getPackage()) {
	    writtenDataType = qualifiedDataType.getName();
	}
	else {
	    writtenDataType = port->getDataType();
	}
	portNode.setAttribute("data-type", writtenDataType);
	portNode.setAttribute("connection-policy", port->getConnectionPolicy() == NodePort::ConnectionPolicy::Multiple ? "multi" : "one");

	writeMetadata(port->getMetadata(), portNode);
	
	portsNode.addChild(portNode);
    }

}

void
SerializerXMLWriter::writeNodeType(
    const std::string & id,
    const NodeType & node_type,
    XmlNodeWrapper nodeTypesNode
    )
{
    XmlNodeWrapper nodeType("node-type");
    nodeType.setAttribute("id", node_type.getLocalId());
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

    writeMetadata(node_type.getMetadata(), nodeType);

    XmlNodeWrapper inputsNode("inputs");
    writeNamedPorts(node_type.getInputs(), inputsNode);
    nodeType.addChild(inputsNode);
    
    XmlNodeWrapper outputsNode("outputs");
    writeNamedPorts(node_type.getOutputs(), outputsNode);
    nodeType.addChild(outputsNode);

    const ConnectionData & defaultNodeData = node_type.getDefaultNodeData();
    if (defaultNodeData.size() != 0) {
	XmlNodeWrapper defaultNodeDataNode("default-node-data");
	writeGraphNodeData(defaultNodeData, defaultNodeDataNode);
	nodeType.addChild(defaultNodeDataNode);
    }
    
    nodeTypesNode.addChild(nodeType);
}

void
SerializerXMLWriter::writeNodeTypes(void)    
{
    XmlNodeWrapper nodeTypesNode("node-types");
    for (const auto & it : mNodeModule.getNodeTypes()) {
	writeNodeType(it.first, *it.second, nodeTypesNode);
    }
    mRoot.addChild(nodeTypesNode);
}

void
SerializerXMLWriter::writeGraphNodeData(
    const ConnectionData & data,
    XmlNodeWrapper nodeNode
    )
{
    for (const auto & it : data.getData()) {
	XmlNodeWrapper valueNode("value");

	valueNode.setAttribute("key", it.first);
	valueNode.setContent(it.second);
	
	nodeNode.addChild(valueNode);
    }
}

void
SerializerXMLWriter::writeGraphNode(
    const Node & node,
    XmlNodeWrapper nodeNode
    )
{
    const NodeType & type = node.getType();
    // If the node type is local to this package, we should omit the package name.
    QualifiedIdentifier qualifiedNodeType(type.getFullyQualifiedId());
    std::string writtenNodeType;
    if (qualifiedNodeType.getNamespace() == mNodeModule.getPackage()) {
	writtenNodeType = qualifiedNodeType.getName();
    }
    else {
	writtenNodeType = type.getFullyQualifiedId();
    }

    nodeNode.setAttribute("type", writtenNodeType);
    writeGraphNodeData(node.getData(), nodeNode);
    
    if (type.getAllowPortOverride()) {
	if (node.hasOverrideInputs()) {
	    XmlNodeWrapper inputsNode("inputs");
	    writeNamedPorts(node.getOverrideInputs(), inputsNode);
	    nodeNode.addChild(inputsNode);
	}
	
	if (node.hasOverrideOutputs()) {
	    XmlNodeWrapper outputsNode("outputs");
	    writeNamedPorts(node.getOverrideOutputs(), outputsNode);
	    nodeNode.addChild(outputsNode);
	}
    }
    
    writeMetadata(node.getMetadata(), nodeNode);
}


void
SerializerXMLWriter::writeGraphNodes(
    const NodeGraph & graph,
    XmlNodeWrapper nodesNode
    )
{
    for (const auto & it : graph.getNodes()) {
	XmlNodeWrapper nodeNode("node");
	const Node &node = *it.second.get();

	nodeNode.setAttribute("id", it.first);
	writeGraphNode(node, nodeNode);
	
	nodesNode.addChild(nodeNode);
    }
    
    
}
    
void
SerializerXMLWriter::writeGraphEdges(
    const NodeGraph & graph,
    XmlNodeWrapper edgesNode
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

void
SerializerXMLWriter::writeGraph(
    std::string id,
    const NodeGraph & graph,
    XmlNodeWrapper graphsNode
    )
{
    XmlNodeWrapper graphNode("graph");
    graphNode.setAttribute("id", id);

    writeMetadata(graph.getMetadata(), graphNode);
    
    XmlNodeWrapper nodesNode("nodes");
    writeGraphNodes(graph, nodesNode);
    graphNode.addChild(nodesNode);

    XmlNodeWrapper edgesNode("edges");
    writeGraphEdges(graph, edgesNode);
    graphNode.addChild(edgesNode);

    graphsNode.addChild(graphNode);
}

void
SerializerXMLWriter::writeGraphs(void)
{
    XmlNodeWrapper graphsNode("graphs");
    for (const auto & it : mNodeModule.getGraphs()) {
	writeGraph(it.first, *it.second, graphsNode);
    }
    mRoot.addChild(graphsNode);
}

bool
SerializerXMLWriter::write(void)
{
    mRoot.setAttribute("xmlns", SerializerXML::NODEJS_XML_NAMESPACE);

    writePackage();
    writeScopes();
    writeDataTypes();
    writeNodeTypes();
    writeGraphs();
    
    xmlDocSetRootElement(mDocument, mRoot.releasePointer());
    

    return true;
}

