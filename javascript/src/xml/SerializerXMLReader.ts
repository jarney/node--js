import type {SerializerErrorReporter} from "../SerializerError.ts";

import {NodeModule} from "../NodeModule.ts";
import {DataType} from "../DataType.ts";
import {NodeType} from "../NodeType.ts";
import {Metadata} from "../Metadata.ts";
import {ConnectionData} from "../ConnectionData.ts";
import type {NodeId} from "../Definitions.ts";
import type {Serializer} from "../Serializer.ts";
import {Node} from "../Node.ts";
import {Edge} from "../Edge.ts";
import {NodePort} from "../NodePort.ts";
import {NodeGraph} from "../NodeGraph.ts";
import {NamedPorts} from "../NamedPorts.ts";
import {SerializerErrorCode, SerializerErrorReporterNil} from "../SerializerError.ts";
import {DomUtils} from "./DomUtils.ts";

export class SerializerXMLReader {
  mDocument: Document;
  mRoot: Element;
  mFilename: string;
  mReporter: SerializerErrorReporter;
  mNodeModule: NodeModule;
  
  constructor(
    aFilename: string,
    aDocument: Document,
    aNodeModule: NodeModule,
    aReporter: SerializerErrorReporter
  ) {
    this.mFilename = aFilename;
    this.mDocument = aDocument;
    this.mNodeModule = aNodeModule;
    this.mReporter = aReporter;
    this.mRoot = aDocument.documentElement;
  }

  public parseError(error_code: number, element: Element, message: string): boolean {
    var lineno: number = DomUtils.getElementLineno(this.mRoot, element);
    this.mReporter.reportError(error_code, lineno, this.mFilename, message);
    return false;
  }

  public parseMetadata(metadataContainer: Element, metadata: Metadata) : boolean {
    for (var metadataElement of DomUtils.getChildrenByTagName(metadataContainer, "metadata")) {
      for (var dataElement of DomUtils.getChildrenByTagName(metadataElement, "data")) {
	var namespace_or_null: string | null = dataElement.getAttribute("namespace");
	if (namespace_or_null == null) {
	  return this.parseError(SerializerErrorCode.METADATA_NAMESPACE_MISSING, dataElement, "Metadata data element is missing 'namespace' attribute.");
	}
	var namespace: string = namespace_or_null!;
	if (metadata.hasMetadata(namespace)) {
	  return this.parseError(SerializerErrorCode.METADATA_NAMESPACE_DUPLICATE, dataElement, "Metadata namespace '" + namespace + "' is duplicated.");
	}
	var cd: ConnectionData = metadata.getMetadata(namespace);
	if (!this.parseConnectionData(
	  dataElement, cd,
	  SerializerErrorCode.METADATA_KEY_MISSING,
	  SerializerErrorCode.METADATA_KEY_DUPLICATE
	)) return false;
      }
    }
    return true;
  }

  public parseConnectionData(
    parent: Element,
    cd: ConnectionData,
    missingCode: SerializerErrorCode,
    duplicateCode: SerializerErrorCode
  ) : boolean {
    for (var valueElement of DomUtils.getChildrenByTagName(parent, "value")) {
      var key_or_null: string | null = valueElement.getAttribute("key");
      if (key_or_null == null) {
	return this.parseError(missingCode, valueElement, "Data value key is missing.");
      }
      var key: string = key_or_null!;
      if (cd.hasValue(key)) {
	return this.parseError(duplicateCode, valueElement, "Data value key '" + key + "' is duplicated.");
      }
      cd.setValue(key, valueElement.textContent);
    }
    return true;
  }
  
  public parsePackage() : boolean {
    var packageCount = 0;

    for (var packageElement of DomUtils.getChildrenByTagName(this.mRoot, "package")) {
      var version: string | null = packageElement.getAttribute("version");
      if (version == null) {
	return this.parseError(SerializerErrorCode.PACKAGE_VERSION_MISSING, packageElement, "Package version is missing");
      }
      else if (version != "1") {
	return this.parseError(SerializerErrorCode.PACKAGE_VERSION_UNSUPPORTED, packageElement, "Package version " + version + " is not yet supported.");
      }
      
      var id: string | null = packageElement.getAttribute("id");
      if (id == null) {
	return this.parseError(SerializerErrorCode.PACKAGE_ID_MISSING, packageElement, "Package element is missing the 'id' element.");
      }
      if (id != this.mFilename) {
	return this.parseError(SerializerErrorCode.PACKAGE_ID_INCONSISTENT, packageElement, "Package id should be the same as the file it was loaded from.");
      }
      this.mNodeModule.setPackage(id!);
      
      // The package description is optional.
      var description: string | null = packageElement.getAttribute("description");
      if (description != null) {
	this.mNodeModule.setDescription(description!);
      }
      if (packageCount > 0) {
	return this.parseError(SerializerErrorCode.PACKAGE_DUPLICATE, packageElement,
	  "Too many package declarations.  There should only be a single package declaration in a module.");
      }
      packageCount++;
    }
    if (packageCount < 1) {
      return this.parseError(SerializerErrorCode.PACKAGE_MISSING, this.mRoot,
	"Package declaration missing.  Module must contain a <package id='...'/> element.");
    }

    // Package-level metadata (optional)
    if (!this.parseMetadata(this.mRoot, this.mNodeModule.getMetadata())) return false;
    
    return true;
  }
  
  
  private parseDataType(dataTypeElement: Element) : boolean {
    var id_or_n: string | null = dataTypeElement.getAttribute("id");
    if (id_or_n == null) {
      return this.parseError(SerializerErrorCode.DATA_TYPE_ID_MISSING, dataTypeElement, "Missing id for data type");
    }
    var id: string = id_or_n!;
    var name: string | null = dataTypeElement.getAttribute("name");
    if (name == null) {
      name = id;
    }
    if (this.mNodeModule.hasDataType(id)) {
      return this.parseError(SerializerErrorCode.DATA_TYPE_ID_DUPLICATE, dataTypeElement, "Duplicate id '" + id + "' for data type.");
    }
    
    var dataType: DataType = new DataType(id, name!);
    this.mNodeModule.addDataType(dataType);
    
    return true;
  }

  public parseDataTypes() : boolean {
    for (var dataTypesElement of DomUtils.getChildrenByTagName(this.mRoot, "data-types")) {
      for (var dataTypeElement of DomUtils.getChildrenByTagName(dataTypesElement, "data-type")) {
	if (!this.parseDataType(dataTypeElement)) return false;
      }
    }
    return true;
  }

  public parsePort(
    portNode: Element,
    namedPorts: NamedPorts,
    desc: string
  ) {
    var id_or_n: string | null = portNode.getAttribute("id");
    if (id_or_n == null) {
      return this.parseError(SerializerErrorCode.PORT_ID_MISSING, portNode, desc + " id missing for " + desc + ".");
    }
    var id: string = id_or_n!;
    if (namedPorts.hasPort(id)) {
      return this.parseError(SerializerErrorCode.PORT_ID_DUPLICATE, portNode, desc + " '" + id + "'missing for " + desc + " is duplicated.");
    }

    var description: string | null = portNode.getAttribute("description");
    if (description != null) {
      description = id;
    }

    var dataType_or_n: string | null = portNode.getAttribute("data-type");
    if (dataType_or_n == null) {
      return this.parseError(SerializerErrorCode.PORT_DATA_TYPE_MISSING, portNode, desc + " '" + id + "' is missing data type.");
    }
    var dataType: string = dataType_or_n!;
    if (!this.mNodeModule.hasDataType(dataType)) {
      return this.parseError(SerializerErrorCode.PORT_DATA_TYPE_INVALID, portNode, desc + " '" + id + "' refers to invalid data type '" + dataType + "'");
    }
    
    var connectionPolicy: NodePort.ConnectionPolicy = NodePort.ConnectionPolicy.One;
    var connectionPolicy_str: string | null = portNode.getAttribute("connection-policy");
    if (connectionPolicy_str == null) {
      connectionPolicy = NodePort.ConnectionPolicy.One;
    }
    else if (connectionPolicy_str == "one") {
      connectionPolicy = NodePort.ConnectionPolicy.One;
    }
    else if (connectionPolicy_str == "multi") {
      connectionPolicy = NodePort.ConnectionPolicy.Multiple;
    }
    else {
      return this.parseError(SerializerErrorCode.PORT_CONNECTION_POLICY_INVALID, portNode, desc + " '" + id + "' has invalid connection policy '" + connectionPolicy_str + "'");
    }
    
    var port: NodePort = new NodePort(dataType, description!, connectionPolicy);
    
    // Node-type metadata (optional)
    if (!this.parseMetadata(portNode, port.getMetadata())) return false;

    namedPorts.addPort(id, port);


    return true;
  }
  
  public parsePorts(
    nodeTypeNode: Element,
    namedPorts: NamedPorts,
    tag: string,
    desc: string) : boolean {
    for (var portsNode of DomUtils.getChildrenByTagName(nodeTypeNode, tag)) {
      for (var portNode of DomUtils.getChildrenByTagName(portsNode, "port")) {
	if (!this.parsePort(portNode, namedPorts, desc)) return false;
      }
    }    
    return true;
  }
  
  public parseNodeType(nodeTypeNode: Element) : boolean {
    var id_or_n: string | null = nodeTypeNode.getAttribute("id");
    if (id_or_n == null) {
      return this.parseError(SerializerErrorCode.NODE_TYPE_ID_MISSING, nodeTypeNode, "Node Type id missing.");
    }
    var id: string = id_or_n!;
    
    if (this.mNodeModule.hasNodeType(id)) {
      return this.parseError(SerializerErrorCode.NODE_TYPE_ID_DUPLICATE, nodeTypeNode, "Duplicate id '" + id + "' for node type.");
    }

    var visibility: NodeType.Visibility = NodeType.Visibility.PRIVATE;

    var visibility_attr: string | null;
    visibility_attr = nodeTypeNode.getAttribute("visibility");
    if (visibility_attr == null) {
      return this.parseError(SerializerErrorCode.NODE_TYPE_VISIBILITY_MISSING, nodeTypeNode, "Visibility missing on node type '" + id + "'");
    }
    else if (visibility_attr == "public") {
      visibility = NodeType.Visibility.PUBLIC;
    }
    else if (visibility_attr == "private") {
      visibility = NodeType.Visibility.PRIVATE;
    }
    else {
      return this.parseError(SerializerErrorCode.NODE_TYPE_VISIBILITY_INVALID, nodeTypeNode, "Visibility qualifier '" + visibility_attr + "' is unrecognized on node type '" + id + "'.");
    }

    var type: NodeType.Type = NodeType.Type.NATIVE;
    var type_attr: string | null = nodeTypeNode.getAttribute("type");
    if (type_attr == null) {
      return this.parseError(SerializerErrorCode.NODE_TYPE_TYPE_MISSING, nodeTypeNode, "Type missing on node type '" + id + "'");
    }
    else if (type_attr == "native") {
      type = NodeType.Type.NATIVE;
    }
    else if (type_attr == "graph") {
      type = NodeType.Type.GRAPH;
    }
    else if (type_attr == "alias") {
      type = NodeType.Type.ALIAS;
    }
    else {
      return this.parseError(SerializerErrorCode.NODE_TYPE_TYPE_INVALID, nodeTypeNode, "Type qualifier '" + type_attr + "' is unrecognized on node type '" + id + "'");
    }

    var nodeType: NodeType = new NodeType();
    nodeType.setId(id);
    nodeType.setVisibility(visibility);
    nodeType.setType(type);

    var override_attribute: string | null = nodeTypeNode.getAttribute("allow-port-override");
    if (override_attribute != null) {
      var allowOverride: boolean = nodeTypeNode.getAttribute("allow-port-override") == "true";
      nodeType.setAllowPortOverride(allowOverride);
    }
    
    if (!this.parsePorts(nodeTypeNode, nodeType.getInputs(), "inputs", "Input Port")) return false;
    if (!this.parsePorts(nodeTypeNode, nodeType.getOutputs(), "outputs", "Output Port")) return false;

    // Default node data.
    if (!this.parseConnectionData(
      nodeTypeNode, nodeType.getDefaultNodeData(),
      SerializerErrorCode.NODE_TYPE_DATA_KEY_MISSING,
      SerializerErrorCode.NODE_TYPE_DATA_KEY_DUPLICATE,
    )) return false;
    
    // Node-type metadata (optional)
    if (!this.parseMetadata(nodeTypeNode, nodeType.getMetadata())) return false;
    
    this.mNodeModule.addNodeType(nodeType);
    
    return true;
  }
  
  public parseNodeTypes() : boolean {
    for (var nodeTypesNode of DomUtils.getChildrenByTagName(this.mRoot, "node-types")) {
      for (var nodeTypeNode of DomUtils.getChildrenByTagName(nodeTypesNode, "node-type")) {
	if (!this.parseNodeType(nodeTypeNode)) return false;
      }
    }
    return true;
  }

  public parseNode(nodeNode: Element, nodeGraph: NodeGraph) : boolean {

    var id_or_n: string | null = nodeNode.getAttribute("id");
    if (id_or_n == null) {
      return this.parseError(SerializerErrorCode.NODE_NODE_ID_MISSING, nodeNode, "Node id missing");
    }
    var nodeId: string = id_or_n!;
    if (nodeGraph.hasNode(nodeId)) {
      return this.parseError(SerializerErrorCode.NODE_NODE_ID_DUPLICATE, nodeNode, "Node id '" + nodeId + "' is duplicated");
    }

    var nodeType_str: string | null = nodeNode.getAttribute("type");
    if (nodeType_str == null) {
      return this.parseError(SerializerErrorCode.NODE_NODE_TYPE_MISSING, nodeNode, "Node id'" + nodeId + "' has missing type.");
    }
    else if (!this.mNodeModule.hasNodeType(nodeType_str)) {
      return this.parseError(SerializerErrorCode.NODE_NODE_TYPE_INVALID, nodeNode, "Node id '" + nodeId + "' has invalid type '" + nodeType_str + "'");
    }
    
    var nodeType = this.mNodeModule.getNodeType(nodeType_str!)!;

    var nodeData: ConnectionData = new ConnectionData();
    if (!this.parseConnectionData(
      nodeNode, nodeData,
      SerializerErrorCode.NODE_DATA_KEY_MISSING,
      SerializerErrorCode.NODE_DATA_KEY_DUPLICATE,
    )) return false;
    
    var node: Node = nodeGraph.newNode(nodeType, nodeId, nodeData);

    if (DomUtils.hasChildrenByTagName(nodeNode, "inputs")) {
      if (!nodeType.getAllowPortOverride()) {
	return this.parseError(SerializerErrorCode.NODE_NODE_TYPE_DOES_NOT_ALLOW_OVERRIDE, nodeNode, "Node id '" + nodeId + "' does not allow port overrides");
      }
      node.setOverrideInputs(true);
      if (!this.parsePorts(nodeNode, node.getOverrideInputs(), "inputs", "Input Port")) return false;
    }

    if (DomUtils.hasChildrenByTagName(nodeNode, "outputs")) {
      if (!nodeType.getAllowPortOverride()) {
	return this.parseError(SerializerErrorCode.NODE_NODE_TYPE_DOES_NOT_ALLOW_OVERRIDE, nodeNode, "Node id '" + nodeId + "' does not allow port overrides");
      }
      node.setOverrideOutputs(true);
      if (!this.parsePorts(nodeNode, node.getOverrideOutputs(), "outputs", "Output Port")) return false;
    }

    // Optional node-level metadata.
    if (!this.parseMetadata(nodeNode, node.getMetadata())) return false;

    return true;
  }

  public parseEdge(edgeNode: Element, nodeGraph: NodeGraph) : boolean {

    var fromNode_or_n: string | null = edgeNode.getAttribute("from-node");
    var fromPort_or_n: string | null = edgeNode.getAttribute("from-port");
    var toNode_or_n: string | null = edgeNode.getAttribute("to-node");
    var toPort_or_n: string | null = edgeNode.getAttribute("to-port");

    if (fromNode_or_n == null) {
      return this.parseError(SerializerErrorCode.EDGE_FROM_NODE_MISSING, edgeNode, "Graph edge is missing 'from-node' attribute");
    }
    if (fromPort_or_n == null) {
      return this.parseError(SerializerErrorCode.EDGE_FROM_PORT_MISSING, edgeNode, "Graph edge is missing 'from-port' attribute");
    }
    if (toNode_or_n == null) {
      return this.parseError(SerializerErrorCode.EDGE_TO_NODE_MISSING, edgeNode, "Graph edge is missing 'to-node' attribute");
    }
    if (toPort_or_n == null) {
      return this.parseError(SerializerErrorCode.EDGE_TO_PORT_MISSING, edgeNode, "Graph edge is missing 'to-port' attribute");
    }
    
    var fromNode: string = fromNode_or_n!;
    var fromPort: string = fromPort_or_n!;
    var toNode: string = toNode_or_n!;
    var toPort: string = toPort_or_n!;

    if (!nodeGraph.hasNode(fromNode)) {
      return this.parseError(SerializerErrorCode.EDGE_FROM_NODE_INVALID, edgeNode, "Graph edge from-node '" + fromNode + "' is not defined in this graph.");      
    }
    var fromNodeObj: Node = nodeGraph.getNode(fromNode)!;
    if (!fromNodeObj.getOutputs().hasPort(fromPort)) {
      return this.parseError(SerializerErrorCode.EDGE_FROM_PORT_INVALID, edgeNode, "Graph edge from-node '" + fromNode + "' of type '" + fromNodeObj.getType().getId() + "' does not have output port '" + fromPort + "'.");
    }
    if (!nodeGraph.hasNode(toNode)) {
      return this.parseError(SerializerErrorCode.EDGE_TO_NODE_INVALID, edgeNode, "Graph edge to-node '" + toNode + "' is not defined in this graph.");      
    }
    var toNodeObj: Node = nodeGraph.getNode(toNode)!;
    if (!toNodeObj.getInputs().hasPort(toPort)) {
      return this.parseError(SerializerErrorCode.EDGE_TO_PORT_INVALID, edgeNode, "Graph edge to-node '" + toNode + "' of type '" + toNodeObj.getType().getId() + "' does not have input port '" + toPort + "'.");
    }

    // Make sure that the data-types in the from and to line up correctly.
    var fromPortObj: NodePort = fromNodeObj.getOutputs().getByName(fromPort)!;
    var toPortObj: NodePort = toNodeObj.getInputs().getByName(toPort)!;

    if (fromPortObj.getDataType() != toPortObj.getDataType()) {
      return this.parseError(SerializerErrorCode.EDGE_PORT_DATA_MISMATCH, edgeNode,
	"Graph edge from-node '" + fromNode + "' of type '" + fromNodeObj.getType().getId() + "' output port '" + fromPort + "' has data type '" + fromPortObj.getDataType() + "', " +
	  " to-node '" + toNode + "' of type '" + toNodeObj.getType().getId() + "' input port '" + toPort + "' has data type '" + toPortObj.getDataType() + "'");
    }

    if (nodeGraph.hasEdge(fromNode, fromPort, toNode, toPort)) {
      return this.parseError(SerializerErrorCode.EDGE_DUPLICATE, edgeNode, "Graph edge '" + fromNode + ":" + fromPort + " -> " + toNode + ":" + toPort + "' is duplicated.");
    }

    // Before adding a new edge, make sure we're not violating the connection policy for a port.
    var edgesFrom: Edge[] = nodeGraph.getEdgesFrom(fromNode);
    var countFromPort: number = 0;
    for (var edge of edgesFrom) {
      if (edge.fromPort == fromPort) {
        countFromPort++;
      }
    }

    if (fromPortObj.getConnectionPolicy() == NodePort.ConnectionPolicy.One) {
      if (countFromPort != 0) {
	return this.parseError(SerializerErrorCode.EDGE_VIOLATES_POLICY, edgeNode, "Node output port '" + fromNode + ":" + fromPort + " only allows a single connection because of the port's policy.");
      }
    }
      
    var countToPort: number = 0;
    var edgesTo: Edge[] = nodeGraph.getEdgesTo(toNode);
    for (var edge of edgesTo) {
      if (edge.toPort == toPort) {
        countToPort++;
      }
    }
    
    if (toPortObj.getConnectionPolicy() == NodePort.ConnectionPolicy.One) {
      if (countToPort != 0) {
	return this.parseError(SerializerErrorCode.EDGE_VIOLATES_POLICY, edgeNode, "Node input port '" + toNode + ":" + toPort + " only allows a single connection because of the port's policy.");
      }
    }
    
    nodeGraph.newEdge(
      fromNode, fromPort,
      toNode, toPort);
    
    return true;
  }

  public parseGraph(graphNode: Element) : boolean {
    var id_or_n: string | null = graphNode.getAttribute("id");
    if (id_or_n == null) {
      return this.parseError(SerializerErrorCode.GRAPH_ID_MISSING, graphNode, "Graph id missing.");
    }
    var id: string = id_or_n!;
    
    if (this.mNodeModule.hasGraph(id)) {
      return this.parseError(SerializerErrorCode.GRAPH_ID_DUPLICATE, graphNode, "Duplicate graph id '" + id + "' for graph.");
    }

    // TODO: Scope resolution needs to be symbolic
    // at this point.  Actual scope resolution needs to happen
    // in a context where we actually have access to a module
    // loader mechanism and this is too low-level for that.
    for (var scopeNode of DomUtils.getChildrenByTagName(graphNode, "scope")) {
      for (var packageNode of DomUtils.getChildrenByTagName(scopeNode, "package")) {
	// TODO: We need to figure out how to unwind the recursiveness here.
      }
    }

    var nodeGraph: NodeGraph = this.mNodeModule.addGraph(id)!;

    for (var nodesNode of DomUtils.getChildrenByTagName(graphNode, "nodes")) {
      for (var nodeNode of DomUtils.getChildrenByTagName(nodesNode, "node")) {
	if (!this.parseNode(nodeNode, nodeGraph)) return false;
      }
    }
    for (var edgesNode of DomUtils.getChildrenByTagName(graphNode, "edges")) {
      for (var edgeNode of DomUtils.getChildrenByTagName(edgesNode, "edge")) {
	if (!this.parseEdge(edgeNode, nodeGraph)) return false;
      }
    }
    var sortedNodes: NodeId[] | undefined = nodeGraph.getNodeIdsInTopologicalOrder();
    if (sortedNodes == undefined) {
      return this.parseError(SerializerErrorCode.GRAPH_CYCLE, graphNode, "Graph '" + id + "' contains a cycle.  This is not permitted.");
    }
    
    return true;
  }
  
  
  public parseGraphs() : boolean {
    for (var graphsNode of DomUtils.getChildrenByTagName(this.mRoot, "graphs")) {
      for (var graphNode of DomUtils.getChildrenByTagName(graphsNode, "graph")) {
	if (!this.parseGraph(graphNode)) return false;
      }
    }
    return true;
  }
  
  public read() : boolean {
    
    if (!this.parsePackage()) return false;

    // Data types need to be read before node types
    // even if they appear later in the file.
    if (!this.parseDataTypes()) return false;

    // Next load the node types since the ports on node types
    // reference data types.
    if (!this.parseNodeTypes()) return false;

    // Next, load the graphs (nodes and edges)
    // which refer to node types and ports.
    if (!this.parseGraphs()) return false;

    return true;
  }
}

