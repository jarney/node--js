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
    , mRoot(xmlDocGetRootElement(mDocument), false)
{}

bool
SerializerXMLReader::parseError(
    SerializerErrorCode error_code,
    const XmlNodeWrapper & element,
    std::string message
    ) const
{
    long lineno =	element.getLineno();
    mReporter.reportError(error_code, lineno, mFilename, message);
    return false;
}

bool
SerializerXMLReader::parseMetadata(
    const XmlNodeWrapper & metadataContainer,
    Metadata & metadata
    ) const
{
    for (XmlNodeWrapper metadataElement : metadataContainer.getChildrenByTagName("metadata")) {
	for (XmlNodeWrapper dataElement : metadataElement.getChildrenByTagName("data")) {
	    if (!dataElement.hasAttribute("namespace")) {
		return parseError(SerializerErrorCode::METADATA_NAMESPACE_MISSING, dataElement, "Metadata data element is missing 'namespace' attribute.");
	    }
	    std::string ns = dataElement.getAttribute("namespace");
	    if (metadata.hasMetadata(ns)) {
		return parseError(SerializerErrorCode::METADATA_NAMESPACE_DUPLICATE, dataElement, "Metadata namespace '" + ns + "' is duplicated.");
	    }
	    ConnectionData & cd = metadata.getMetadata(ns);
	    if (!parseConnectionData(
		    dataElement,
		    cd,
		    SerializerErrorCode::METADATA_KEY_MISSING,
		    SerializerErrorCode::METADATA_KEY_DUPLICATE
		    )) return false;
	}
    }
    return true;
}

bool
SerializerXMLReader::parseConnectionData(
    XmlNodeWrapper parent,
    ConnectionData & cd,
    SerializerErrorCode missingCode,
    SerializerErrorCode duplicateCode
    ) const
{
    for (XmlNodeWrapper valueElement : parent.getChildrenByTagName("value")) {
	if (!valueElement.hasAttribute("key")) {
	    return parseError(missingCode, valueElement, "Data value key is missing.");
	}
	std::string key = valueElement.getAttribute("key");
	if (cd.hasValue(key)) {
	    return parseError(duplicateCode, valueElement, "Data value key '" + key + "' is duplicated.");
	}
	cd.setValue(key, valueElement.getContent());
    }
    return true;
}

bool
SerializerXMLReader::parsePackage() const
{
    int packageCount = 0;
    
    for (XmlNodeWrapper packageElement : mRoot.getChildrenByTagName("package")) {
	if (!packageElement.hasAttribute("version")) {
	    return parseError(SerializerErrorCode::PACKAGE_VERSION_MISSING, packageElement, "Package version is missing");
	}
	std::string version = packageElement.getAttribute("version");
	if (version != "1") {
	    return parseError(SerializerErrorCode::PACKAGE_VERSION_UNSUPPORTED, packageElement, "Package version " + version + " is not yet supported.");
	}

	std::string id;
	if (!packageElement.hasAttribute("id")) {
	    mReporter.reportError(SerializerErrorCode::PACKAGE_ID_MISSING, 22, "context", std::string("Package 'id' is missing"));
	    return false;
	}
	id = packageElement.getAttribute("id");
	if (id != mFilename) {
	    return parseError(SerializerErrorCode::PACKAGE_ID_INCONSISTENT, packageElement, "Package id should be the same as the file it was loaded from.");
	}
	
	mNodeModule.setPackage(id);
	
	if (packageElement.hasAttribute("description")) {
	    std::string desc = packageElement.getAttribute("description");
	    mNodeModule.setDescription(desc);
	}
	else {
	    mNodeModule.setDescription(id);
	}
	if (packageCount > 0) {
	    return parseError(SerializerErrorCode::PACKAGE_DUPLICATE, packageElement,
			      "Too many package declarations.  There should only be a single package declaration in a module.");
	}
	packageCount++;
    }
    if (packageCount < 1) {
      return parseError(SerializerErrorCode::PACKAGE_MISSING, mRoot,
			"Package declaration missing.  Module must contain a <package id='...'/> element.");
    }

    if (!parseMetadata(mRoot, mNodeModule.getMetadata())) return false;

    return true;
}

bool
SerializerXMLReader::parseDataType(
    XmlNodeWrapper dataTypeElement
    ) const
{
    if (!dataTypeElement.hasAttribute("id")) {
	return parseError(SerializerErrorCode::DATA_TYPE_ID_MISSING, dataTypeElement, "Missing id for data type");
    }
    std::string id = dataTypeElement.getAttribute("id");
    std::string name;
    if (dataTypeElement.hasAttribute("name")) {
	name = dataTypeElement.getAttribute("name");
    }
    else {
	name = id;
    }
    if (mNodeModule.hasDataType(id)) {
	return parseError(SerializerErrorCode::DATA_TYPE_ID_DUPLICATE, dataTypeElement, "Duplicate id '" + id + "' for data type.");
    }
    
    std::unique_ptr<DataType> dataType = std::make_unique<DataType>(id, name);
    mNodeModule.addDataType(std::move(dataType));
    
    return true;

    
    return true;
}

bool
SerializerXMLReader::parseDataTypes() const
{
    for (XmlNodeWrapper dataTypesElement : mRoot.getChildrenByTagName("data-types")) {
	for (XmlNodeWrapper dataTypeElement : dataTypesElement.getChildrenByTagName("data-type")) {
	    if (!parseDataType(dataTypeElement)) return false;
	}
    }
    return true;
}

bool SerializerXMLReader::parsePort(
    XmlNodeWrapper portNode,
    NodeJS::core::NamedPorts & namedPorts,
    std::string desc,
    const NodeGraph *graph
    ) const
{
    if (!portNode.hasAttribute("id")) {
	return parseError(SerializerErrorCode::PORT_ID_MISSING, portNode, desc + " id missing for " + desc + ".");
    }

    std::string id = portNode.getAttribute("id");
    if (namedPorts.hasPort(id)) {
	return parseError(SerializerErrorCode::PORT_ID_DUPLICATE, portNode, desc + " '" + id + "'missing for " + desc + " is duplicated.");
    }

    std::string description;
    if (portNode.hasAttribute("description")) {
	description = portNode.getAttribute("description");
    }
    else {
	description = id;
    }

    if (!portNode.hasAttribute("data-type")) {
	return parseError(SerializerErrorCode::PORT_DATA_TYPE_MISSING, portNode, desc + " '" + id + "' is missing data type.");
    }
    std::string dataType = portNode.getAttribute("data-type");
    bool hasDataType = mNodeModule.hasDataType(dataType);
    if (graph) {
	hasDataType = graph->hasDataType(dataType);
    }
    if (!hasDataType) {
	return parseError(SerializerErrorCode::PORT_DATA_TYPE_INVALID, portNode, desc + " '" + id + "' refers to invalid data type '" + dataType + "'");
    }
    
    NodePort::ConnectionPolicy connectionPolicy = NodePort::ConnectionPolicy::One;
    if (portNode.hasAttribute("connection-policy")) {
	std::string connectionPolicy_str = portNode.getAttribute("connection-policy");
	if (connectionPolicy_str == "one") {
	    connectionPolicy = NodePort::ConnectionPolicy::One;
	}
	else if (connectionPolicy_str == "multi") {
	    connectionPolicy = NodePort::ConnectionPolicy::Multiple;
	}
	else {
	    return parseError(SerializerErrorCode::PORT_CONNECTION_POLICY_INVALID, portNode, desc + " '" + id + "' has invalid connection policy '" + connectionPolicy_str + "'");
	}
    }
    
    std::unique_ptr<NodePort> port = std::make_unique<NodePort>(dataType, description, connectionPolicy);
    
    // Node-type metadata (optional)
    if (!parseMetadata(portNode, port->getMetadata())) return false;

    namedPorts.addPort(id, std::move(port));

    return true;
}
  
bool
SerializerXMLReader::parsePorts(
    XmlNodeWrapper nodeTypeNode,
    NodeJS::core::NamedPorts & namedPorts,
    std::string tag,
    std::string desc,
    const NodeGraph *graph
    ) const
{
    for (XmlNodeWrapper portsNode : nodeTypeNode.getChildrenByTagName(tag)) {
	for (XmlNodeWrapper portNode : portsNode.getChildrenByTagName("port")) {
	    if (!parsePort(portNode, namedPorts, desc, graph)) return false;
	}
    }
    return true;
}



bool
SerializerXMLReader::parseNodeType(
    XmlNodeWrapper nodeTypeNode
    ) const
{
    if (!nodeTypeNode.hasAttribute("id")) {
	return parseError(SerializerErrorCode::NODE_TYPE_ID_MISSING, nodeTypeNode, "Node Type id missing.");
    }
    
    std::string id = nodeTypeNode.getAttribute("id");
    if (mNodeModule.hasNodeType(id)) {
	return parseError(SerializerErrorCode::NODE_TYPE_ID_DUPLICATE, nodeTypeNode, "Duplicate id '" + id + "' for node type.");
    }

    if (!nodeTypeNode.hasAttribute("visibility")) {
	return parseError(SerializerErrorCode::NODE_TYPE_VISIBILITY_MISSING, nodeTypeNode, "Visibility missing on node type '" + id + "'");
    }
    std::string visibility_attr = nodeTypeNode.getAttribute("visibility");
    NodeType::Visibility visibility;
    if (visibility_attr == std::string("public")) {
	visibility = NodeType::Visibility::PUBLIC;
    }
    else if (visibility_attr == "private") {
	visibility = NodeType::Visibility::PRIVATE;
    }
    else {
	return parseError(SerializerErrorCode::NODE_TYPE_VISIBILITY_INVALID, nodeTypeNode, "Visibility qualifier '" + visibility_attr + "' is unrecognized on node type '" + id + "'.");
    }

    if (!nodeTypeNode.hasAttribute("type")) {
	return parseError(SerializerErrorCode::NODE_TYPE_TYPE_MISSING, nodeTypeNode, "Type missing on node type '" + id + "'");
    }
    std::string type_attr = nodeTypeNode.getAttribute("type");
    NodeType::Type type;
    if (type_attr == "native") {
	type = NodeType::Type::NATIVE;
    }
    else if (type_attr == "graph") {
	type = NodeType::Type::GRAPH;
    }
    else if (type_attr == "alias") {
	type = NodeType::Type::ALIAS;
    }
    else {
	return parseError(SerializerErrorCode::NODE_TYPE_TYPE_INVALID, nodeTypeNode, "Type qualifier '" + type_attr + "' is unrecognized on node type '" + id + "'");
    }

    std::unique_ptr<NodeType> nodeType = std::make_unique<NodeType>();
    nodeType->setId(id);
    nodeType->setVisibility(visibility);
    nodeType->setType(type);

    if (nodeTypeNode.hasAttribute("allow-port-override")) {
	bool allowOverride = nodeTypeNode.getAttribute("allow-port-override") == "true";
	nodeType->setAllowPortOverride(allowOverride);
    }
    
    if (!parsePorts(nodeTypeNode, nodeType->getInputs(), "inputs", "Input Port", nullptr)) return false;
    if (!parsePorts(nodeTypeNode, nodeType->getOutputs(), "outputs", "Output Port", nullptr)) return false;

    ConnectionData node_data;
    if (!parseConnectionData(
	    nodeTypeNode, node_data,
	    SerializerErrorCode::NODE_TYPE_DATA_KEY_MISSING,
	    SerializerErrorCode::NODE_TYPE_DATA_KEY_DUPLICATE
	    )) return false;
    nodeType->setDefaultNodeData(node_data);
    
    if (!parseMetadata(nodeTypeNode, nodeType->getMetadata())) return false;
    
    mNodeModule.addNodeType(std::move(nodeType));

    return true;
}

bool
SerializerXMLReader::parseNodeTypes(void) const
{
    for (XmlNodeWrapper nodeTypesNode : mRoot.getChildrenByTagName("node-types")) {
	for (XmlNodeWrapper nodeTypeNode : nodeTypesNode.getChildrenByTagName("node-type")) {
	    if (!parseNodeType(nodeTypeNode)) return false;
	}
    }
    return true;
}

bool
SerializerXMLReader::parseNode(
    XmlNodeWrapper nodeNode,
    NodeGraph & nodeGraph
    ) const
{

    if (!nodeNode.hasAttribute("id")) {
	return parseError(SerializerErrorCode::NODE_NODE_ID_MISSING, nodeNode, "Node id missing");
    }
    std::string nodeId = nodeNode.getAttribute("id");
    if (nodeGraph.hasNode(nodeId)) {
	return parseError(SerializerErrorCode::NODE_NODE_ID_DUPLICATE, nodeNode, "Node id '" + nodeId + "' is duplicated");
    }
    if (!nodeNode.hasAttribute("type")) {
	return parseError(SerializerErrorCode::NODE_NODE_TYPE_MISSING, nodeNode, "Node id'" + nodeId + "' has missing type.");
    }
    std::string nodeType_str = nodeNode.getAttribute("type");
    const NodeType *nodeType = nodeGraph.getNodeType(nodeType_str);
    if (nodeType == nullptr) {
	return parseError(SerializerErrorCode::NODE_NODE_TYPE_INVALID, nodeNode, "Node id '" + nodeId + "' has invalid type '" + nodeType_str + "'");
    }

    ConnectionData nodeData;
    if (!parseConnectionData(
	    nodeNode, nodeData,
	    SerializerErrorCode::NODE_DATA_KEY_MISSING,
	    SerializerErrorCode::NODE_DATA_KEY_DUPLICATE
	    )) return false;
    
    Node & node = nodeGraph.newNode(
	*nodeType,
	nodeId,
	nodeData
	);

    if (nodeNode.hasChildrenByTagName("inputs")) {
	if (!nodeType->getAllowPortOverride()) {
	    return parseError(SerializerErrorCode::NODE_NODE_TYPE_DOES_NOT_ALLOW_OVERRIDE, nodeNode, "Node id '" + nodeId + "' does not allow port overrides");
	}
	node.setOverrideInputs(true);
	if (!parsePorts(nodeNode, node.getOverrideInputs(), "inputs", "Input Port", &nodeGraph)) return false;
    }
    
    if (nodeNode.hasChildrenByTagName("outputs")) {
	if (!nodeType->getAllowPortOverride()) {
	    return parseError(SerializerErrorCode::NODE_NODE_TYPE_DOES_NOT_ALLOW_OVERRIDE, nodeNode, "Node id '" + nodeId + "' does not allow port overrides");
	}
	node.setOverrideOutputs(true);
	if (!parsePorts(nodeNode, node.getOverrideOutputs(), "outputs", "Output Port", &nodeGraph)) return false;
    }
    
    // Optional node-level metadata.
    if (!parseMetadata(nodeNode, node.getMetadata())) return false;
    
    return true;
}

bool
SerializerXMLReader::parseEdge(
    XmlNodeWrapper edgeNode,
    NodeGraph & nodeGraph
    ) const
{

    if (!edgeNode.hasAttribute("from-node")) {
      return parseError(SerializerErrorCode::EDGE_FROM_NODE_MISSING, edgeNode, "Graph edge is missing 'from-node' attribute");
    }
    if (!edgeNode.hasAttribute("from-port")) {
      return parseError(SerializerErrorCode::EDGE_FROM_PORT_MISSING, edgeNode, "Graph edge is missing 'from-port' attribute");
    }
    if (!edgeNode.hasAttribute("to-node")) {
      return parseError(SerializerErrorCode::EDGE_TO_NODE_MISSING, edgeNode, "Graph edge is missing 'to-node' attribute");
    }
    if (!edgeNode.hasAttribute("to-port")) {
      return parseError(SerializerErrorCode::EDGE_TO_PORT_MISSING, edgeNode, "Graph edge is missing 'to-port' attribute");
    }
    
    std::string fromNode = edgeNode.getAttribute("from-node");
    std::string toNode = edgeNode.getAttribute("to-node");
    std::string fromPort = edgeNode.getAttribute("from-port");
    std::string toPort = edgeNode.getAttribute("to-port");

    if (!nodeGraph.hasNode(fromNode)) {
      return parseError(SerializerErrorCode::EDGE_FROM_NODE_INVALID, edgeNode, "Graph edge from-node '" + fromNode + "' is not defined in this graph.");      
    }
    Node & fromNodeObj = *nodeGraph.getNode(fromNode);
    if (!fromNodeObj.getOutputs().hasPort(fromPort)) {
      return parseError(SerializerErrorCode::EDGE_FROM_PORT_INVALID, edgeNode, "Graph edge from-node '" + fromNode + "' of type '" + fromNodeObj.getType().getId() + "' does not have output port '" + fromPort + "'.");
    }
    if (!nodeGraph.hasNode(toNode)) {
      return parseError(SerializerErrorCode::EDGE_TO_NODE_INVALID, edgeNode, "Graph edge to-node '" + toNode + "' is not defined in this graph.");      
    }
    Node & toNodeObj = *nodeGraph.getNode(toNode);
    if (!toNodeObj.getInputs().hasPort(toPort)) {
      return parseError(SerializerErrorCode::EDGE_TO_PORT_INVALID, edgeNode, "Graph edge to-node '" + toNode + "' of type '" + toNodeObj.getType().getId() + "' does not have input port '" + toPort + "'.");
    }

    // Make sure that the data-types in the from and to line up correctly.
    const NodePort & fromPortObj = *fromNodeObj.getOutputs().getByName(fromPort);
    const NodePort & toPortObj = *toNodeObj.getInputs().getByName(toPort);

    if (fromPortObj.getDataType() != toPortObj.getDataType()) {
	return parseError(SerializerErrorCode::EDGE_PORT_DATA_MISMATCH, edgeNode,
			  "Graph edge from-node '" + fromNode + "' of type '" + fromNodeObj.getType().getId() + "' output port '" + fromPort + "' has data type '" + fromPortObj.getDataType() + "', " +
			  " to-node '" + toNode + "' of type '" + toNodeObj.getType().getId() + "' input port '" + toPort + "' has data type '" + toPortObj.getDataType() + "'");
    }

    if (nodeGraph.hasEdge(fromNode, fromPort, toNode, toPort)) {
	return parseError(SerializerErrorCode::EDGE_DUPLICATE, edgeNode, "Graph edge '" + fromNode + ":" + fromPort + " -> " + toNode + ":" + toPort + "' is duplicated.");
    }

    // Before adding a new edge, make sure we're not violating the connection policy for a port.
    const std::vector<const Edge *> & edgesFrom = nodeGraph.getEdgesFrom(fromNode);
    size_t countFromPort = 0;
    for (const auto edge : edgesFrom) {
      if (edge->fromPort == fromPort) {
        countFromPort++;
      }
    }

    if (fromPortObj.getConnectionPolicy() == NodePort::ConnectionPolicy::One) {
	if (countFromPort != 0) {
	    return parseError(SerializerErrorCode::EDGE_VIOLATES_POLICY, edgeNode, "Node output port '" + fromNode + ":" + fromPort + " only allows a single connection because of the port's policy.");
	}
    }
      
    size_t countToPort = 0;
    const std::vector<const Edge *> & edgesTo = nodeGraph.getEdgesTo(toNode);
    for (const auto edge : edgesTo) {
	if (edge->toPort == toPort) {
        countToPort++;
      }
    }
    
    if (toPortObj.getConnectionPolicy() == NodePort::ConnectionPolicy::One) {
      if (countToPort != 0) {
	  return parseError(SerializerErrorCode::EDGE_VIOLATES_POLICY, edgeNode, "Node input port '" + toNode + ":" + toPort + " only allows a single connection because of the port's policy.");
      }
    }
    
    nodeGraph.newEdge(
	fromNode, fromPort,
	toNode, toPort
	);

    return true;
}

bool
SerializerXMLReader::readGraphScope(
    XmlNodeWrapper packageNode,
    NodeGraph & nodeGraph
    ) const
{
    ModuleLoader & loader = mNodeModule.getModuleLoader();
    std::string id = packageNode.getAttribute("id");
    const NodeModule *scope_package = loader.loadModule(id, mReporter);
    if (scope_package) {
	fprintf(stderr, "Loaded package %s\n", id.c_str());
	nodeGraph.addScope(scope_package);
    }
    else {
	fprintf(stderr, "Failed to load scope %s\n", packageNode.getAttribute("id").c_str());
	return false;
    }

    return true;
}

bool
SerializerXMLReader::parseGraph(
    XmlNodeWrapper graphNode
    ) const
{

    if (!graphNode.hasAttribute("id")) {
	return parseError(SerializerErrorCode::GRAPH_ID_MISSING, graphNode, "Graph id missing.");
    }
    std::string id = graphNode.getAttribute("id");
    
    if (mNodeModule.hasGraph(id)) {
	return parseError(SerializerErrorCode::GRAPH_ID_DUPLICATE, graphNode, "Duplicate graph id '" + id + "' for graph.");
    }

    NodeGraph & nodeGraph = *mNodeModule.addGraph(id);

    for (XmlNodeWrapper scopeNode : graphNode.getChildrenByTagName("scope")) {
	for (XmlNodeWrapper packageNode : scopeNode.getChildrenByTagName("package")) {
	    readGraphScope(packageNode, nodeGraph);
      }
    }

    
    parseMetadata(graphNode, nodeGraph.getMetadata());
    
    for (XmlNodeWrapper nodesNode : graphNode.getChildrenByTagName("nodes")) {
	for (XmlNodeWrapper nodeNode : nodesNode.getChildrenByTagName("node")) {
	    if (!parseNode(nodeNode, nodeGraph)) return false;
	}
    }
    for (XmlNodeWrapper edgesNode : graphNode.getChildrenByTagName("edges")) {
	for (XmlNodeWrapper edgeNode : edgesNode.getChildrenByTagName("edge")) {
	    if (!parseEdge(edgeNode, nodeGraph)) return false;
	}
    }
    
    std::optional<std::vector<NodeId>> maybe_sorted = nodeGraph.getNodeIdsInTopologicalOrder();
    if (!maybe_sorted.has_value()) {
	return parseError(SerializerErrorCode::GRAPH_CYCLE, graphNode, "Graph '" + id + "' contains a cycle.  This is not permitted.");
    }

    return true;
}

bool
SerializerXMLReader::parseGraphs(void) const
{
    for (XmlNodeWrapper graphsNode : mRoot.getChildrenByTagName("graphs")) {
	for (XmlNodeWrapper graphNode : graphsNode.getChildrenByTagName("graph")) {
	    if (!parseGraph(graphNode)) return false;
	}
    }
    return true;
}

bool
SerializerXMLReader::read(void) const
{
    // The package needs to be read first because that determines where
    // other data is registered.
    if (!parsePackage()) return false;

    // Data types need to be read before node types
    // even if they appear later in the file.
    if (!parseDataTypes()) return false;
    
    // Node types must be loaded before we load any nodes
    if (!parseNodeTypes()) return false;

    // Next, load the graphs (nodes and edges)
    // which refer to node types and ports.
    if (!parseGraphs()) return false;

    return true;
}    

