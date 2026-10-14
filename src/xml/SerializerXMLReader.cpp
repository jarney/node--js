#include "SerializerXMLReader.hpp"
#include "node--js/ModuleLoader.hpp"
#include "node--js/Metadata.hpp"

using namespace NodeJS::xml;
using namespace NodeJS::core;

SerializerXMLReader::SerializerXMLReader(
    std::string aFilename,
    xmlDocPtr aDocument,
    NodeModule & aNodeModule,
    SerializerErrorReporter & aErrorReporter
    )
    : mFilename(aFilename)
    , mDocument(aDocument)
    , mNodeModule(aNodeModule)
    , mReporter(aErrorReporter)
{}

bool
SerializerXMLReader::parseError(
    SerializerErrorCode error_code,
    XmlNodeWrapper & element,
    std::string message
    ) const
{
    long lineno =	element.getLineno();
    mReporter.reportError(error_code, lineno, mFilename, message);
    return false;
}

bool
SerializerXMLReader::parseMetadata(
    XmlNodeWrapper & metadataContainer,
    Metadata & metadata
    ) const
{
    for (XmlNodeWrapper dataElement : metadataContainer.getChildrenByTagName("data")) {
	if (!dataElement.hasAttribute("namespace")) {
	    return parseError(SerializerErrorCode::METADATA_NAMESPACE_MISSING, dataElement, "Metadata data element is missing 'namespace' attribute.");
	}
	std::string ns = dataElement.getAttribute("namespace");
	if (metadata.hasMetadata(ns)) {
	    return parseError(SerializerErrorCode::METADATA_NAMESPACE_DUPLICATE, dataElement, "Metadata namespace '" + ns + "' is duplicated.");
	}
	ConnectionData & cd = metadata.getMetadata(ns);
	for (XmlNodeWrapper valueElement : metadataContainer.getChildrenByTagName("value")) {
	    if (!valueElement.hasAttribute("key")) {
		return parseError(SerializerErrorCode::METADATA_KEY_MISSING, valueElement, "Metadata key is missing.");
	    }
	    std::string key = valueElement.getAttribute("key");
	    if (cd.hasValue(key)) {
		return parseError(SerializerErrorCode::METADATA_KEY_DUPLICATE, valueElement, "Metadata key '" + key + "' is duplicated.");
	    }
	    cd.setValue(key, valueElement.getContent());
	}
    }
    return true;
}

bool
SerializerXMLReader::readDataType(
    XmlNodeWrapper node
    ) const
{
    std::string id = node.getAttribute("id");
    std::string name = node.getAttribute("name");
    std::unique_ptr<DataType> dataType = std::make_unique<DataType>(id, name);
    mNodeModule.addDataType(std::move(dataType));
    
    return true;
}

bool
SerializerXMLReader::readDataTypes(
    XmlNodeWrapper dataTypesNode
    ) const
{
    for (XmlNodeWrapper::Iterator it = dataTypesNode.begin(); it != dataTypesNode.end(); ++it) {
	XmlNodeWrapper child = it.get();
	if (child.getName() != std::string("data-type")) {
	    continue;
	}
	readDataType(child);
    }
    return true;
}

bool
SerializerXMLReader::readPackage(
    XmlNodeWrapper packageNode
    ) const
{
    std::string id;
    if (!packageNode.hasAttribute("id")) {
	mReporter.reportError(SerializerErrorCode::PACKAGE_VERSION_MISSING, 22, "context", std::string("Package 'id' is missing"));
	return false;
    }
    id = packageNode.getAttribute("id");
    mNodeModule.setPackage(id);

    if (packageNode.hasAttribute("description")) {
	std::string desc = packageNode.getAttribute("description");
	mNodeModule.setDescription(desc);
    }
    else {
	mNodeModule.setDescription(id);
    }
    return true;
}

bool
SerializerXMLReader::readNamedPorts(
    NamedPorts & namedPorts,
    XmlNodeWrapper inputsNode
    ) const
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
		parseMetadata(portChild, port->getMetadata());
	    }
	}
	
	namedPorts.addPort(id, std::move(port));
    }
    return true;
}

bool
SerializerXMLReader::readNodeType(
    XmlNodeWrapper nodeTypeNode
    ) const
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
	    readNamedPorts(nodeType.get()->getInputs(), child);
	}
	else if (childName == "outputs") {
	    readNamedPorts(nodeType.get()->getOutputs(), child);
	}
	else if (childName == "default-node-data") {
	    ConnectionData node_data;
	    readGraphNodeData(node_data, child);
	    nodeType->setDefaultNodeData(node_data);
	}
	else if (childName == std::string("metadata")) {
	    parseMetadata(child, nodeType.get()->getMetadata());
	}
	else {
	    // Nothing to do, this is extraneous.
	}
    
    }
    
    mNodeModule.addNodeType(std::move(nodeType));
    return true;
}

bool
SerializerXMLReader::readNodeTypes(
    XmlNodeWrapper nodeTypesNode
    ) const
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
	readNodeType(child);
    }
    return true;
}

bool
SerializerXMLReader::readGraphNodeData(
    ConnectionData & node_data,
    XmlNodeWrapper nodeNode
    ) const
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
    return true;
}

bool
SerializerXMLReader::readGraphNode(
    NodeGraph & node_graph,
    XmlNodeWrapper nodeNode
    ) const
{
    std::string nodeTypeName = nodeNode.getAttribute("type");
    const NodeType *nodeType = node_graph.getNodeType(nodeTypeName);
    if (nodeType == nullptr) {
	mReporter.reportError(SerializerErrorCode::LEGACY_ERROR_XML_PARSE, 22, "context", std::string("Node type ") + nodeTypeName + std::string(" does not exist"));
	return false;
    }
    
    std::string nodeId = nodeNode.getAttribute("id");

    ConnectionData node_data;
    readGraphNodeData(node_data, nodeNode);

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
		readNamedPorts(node.getOverrideInputs(), child);
	    }
	    if (childName == std::string("outputs")) {
		node.setOverrideOutputs(true);
		readNamedPorts(node.getOverrideOutputs(), child);
	    }
	}
	if (childName == std::string("metadata")) {
	    parseMetadata(child, node.getMetadata());
	}
    }
    return true;
}

bool
SerializerXMLReader::readGraphNodes(
    NodeGraph & node_graph,
    XmlNodeWrapper nodesNode
    ) const
{
    for (XmlNodeWrapper::Iterator it = nodesNode.begin(); it != nodesNode.end(); ++it) {
	XmlNodeWrapper child = it.get();
	std::string childName = child.getName();
	if (childName == std::string("node")) {
	    readGraphNode(node_graph, child);
	}

    }
    return true;
}

bool
SerializerXMLReader::readGraphEdge(
    NodeGraph & node_graph,
    XmlNodeWrapper edgeNode
    ) const
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
	mReporter.reportError(SerializerErrorCode::LEGACY_ERROR_XML_PARSE, 0, "Input Stream", "Duplicate graph edge");
	return false;
    }
    return true;
}

bool
SerializerXMLReader::readGraphEdges(
    NodeGraph & node_graph,
    XmlNodeWrapper nodesNode
    )const
{
    for (XmlNodeWrapper::Iterator it = nodesNode.begin(); it != nodesNode.end(); ++it) {
	XmlNodeWrapper child = it.get();
	std::string childName = child.getName();
	if (childName == std::string("edge")) {
	    readGraphEdge(node_graph, child);
	}
    }
    return true;
}

bool
SerializerXMLReader::readGraphScope(
    NodeGraph & node_graph,
    XmlNodeWrapper scopeNode
    ) const
{
    for (XmlNodeWrapper::Iterator it = scopeNode.begin(); it != scopeNode.end(); ++it) {
	XmlNodeWrapper child = it.get();
	std::string childName = child.getName();
	if (childName == std::string("package")) {
	    ModuleLoader & loader = mNodeModule.getModuleLoader();
	    const NodeModule *scope_package = loader.loadModule(child.getAttribute("id"), mReporter);
	    if (scope_package) {
		node_graph.addScope(scope_package);
	    }
	    else {
		fprintf(stderr, "Failed to load scope %s\n", child.getAttribute("id").c_str());
		return false;
	    }
	}
    }
    return true;
}

bool
SerializerXMLReader::readGraph(
    XmlNodeWrapper graphNode
    ) const
{
    std::string id = graphNode.getAttribute("id");
    NodeGraph *graph = mNodeModule.addGraph(id);

    // Scopes must be read first because
    // we may need to resolve node types
    // using the scope if they are externally
    // defined.  How will we deal with this
    // if we need to recursively load?
    for (XmlNodeWrapper::Iterator it = graphNode.begin(); it != graphNode.end(); ++it) {
	XmlNodeWrapper child = it.get();
	std::string childName = child.getName();
	if (childName == std::string("scope")) {
	    readGraphScope(*graph, child);
	}
	else if (childName == std::string("metadata")) {
	    parseMetadata(child, graph->getMetadata());
	}
    }
    
    // All nodes must be read before any edges
    for (XmlNodeWrapper::Iterator it = graphNode.begin(); it != graphNode.end(); ++it) {
	XmlNodeWrapper child = it.get();
	std::string childName = child.getName();
	if (childName == std::string("nodes")) {
	    readGraphNodes(*graph, child);
	}
    }
    for (XmlNodeWrapper::Iterator it = graphNode.begin(); it != graphNode.end(); ++it) {
	XmlNodeWrapper child = it.get();
	std::string childName = child.getName();
	if (childName == std::string("edges")) {
	    readGraphEdges(*graph, child);
	}
    }    
    return true;
}

bool
SerializerXMLReader::readGraphs(
    XmlNodeWrapper nodeTypesNode
    ) const
{
    for (XmlNodeWrapper::Iterator it = nodeTypesNode.begin(); it != nodeTypesNode.end(); ++it) {
	XmlNodeWrapper child = it.get();
	std::string graphName = child.getName();
	if (graphName != std::string("graph")) {
	    continue;
	}
	if (!child.hasAttribute("id")) {
	    mReporter.reportError(SerializerErrorCode::LEGACY_ERROR_XML_PARSE, 0, "Input Stream", "Invalid file, graph without id");
	    continue;
	}
	readGraph(child);
    }
    return true;
}

bool
SerializerXMLReader::read(void) const
{
    /*Get the root element node */
    XmlNodeWrapper root(xmlDocGetRootElement(mDocument), false);

    // The package needs to be read first because that determines where
    // other data is registered.
    for (XmlNodeWrapper::Iterator it = root.begin(); it != root.end(); ++it) {
	XmlNodeWrapper child = it.get();
	std::string childName = child.getName();
	if (childName == std::string("package")) {
	    if (!readPackage(child)) return false;
	}
    }

    // Data types need to be read before node types
    // even if they appear later in the file.
    for (XmlNodeWrapper::Iterator it = root.begin(); it != root.end(); ++it) {
	XmlNodeWrapper child = it.get();
	std::string childName = child.getName();
	if (childName == std::string("data-types")) {
	    readDataTypes(child);
	}
	else if (childName == std::string("metadata")) {
	    parseMetadata(child, mNodeModule.getMetadata());
	}
    }
    
    // Node types must be loaded before we load any nodes
    for (XmlNodeWrapper::Iterator it = root.begin(); it != root.end(); ++it) {
	XmlNodeWrapper child = it.get();
	std::string childName = child.getName();
	if (childName == std::string("node-types")) {
	    readNodeTypes(child);
	}
    }

    // Finally, we have enough qualified metadata
    // the node types and graphs from the file.
    for (XmlNodeWrapper::Iterator it = root.begin(); it != root.end(); ++it) {
	XmlNodeWrapper child = it.get();
	std::string childName = child.getName();
	if (childName == std::string("graphs")) {
	    readGraphs(child);
	}
    }
    
    return true;
}    

