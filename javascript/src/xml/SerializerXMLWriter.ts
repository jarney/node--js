import type {SerializerErrorReporter} from "../SerializerError";
import {NodeModule} from "../NodeModule";
import {ModuleLoader} from "../ModuleLoader";
import {DataType} from "../DataType";
import {NodeType} from "../NodeType";
import {Metadata} from "../Metadata";
import {ConnectionData} from "../ConnectionData";
import type {NodeId} from "../Definitions";
import type {Serializer} from "../Serializer";
import {Node} from "../Node";
import {Edge} from "../Edge";
import {NodePort} from "../NodePort";
import {NodeGraph} from "../NodeGraph";
import {NamedPorts} from "../NamedPorts";
import {QualifiedIdentifier} from "../QualifiedIdentifier";
import {SerializerErrorCode, SerializerErrorReporterNil} from "../SerializerError";
import {DomUtils} from "./DomUtils";
import {SerializerXML} from "./SerializerXML";

export class SerializerXMLWriter {
  mFilename: string;
  mDocument: Document;
  mNodeModule: NodeModule;
  mReporter: SerializerErrorReporter;
  mRoot: Element;

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
    
    this.mRoot = this.mDocument.createElement("node-module");
  }

  public writePackage() {
    var packageEl: Element = this.mDocument.createElement("package");
    packageEl.setAttribute("id", this.mNodeModule.getPackage());
    packageEl.setAttribute("version", "1");
    packageEl.setAttribute("description", this.mNodeModule.getDescription());
    this.mRoot.appendChild(packageEl);

    this.writeMetadata(this.mNodeModule.getMetadata(), this.mRoot);
  }
  public writeScopes() {
    var scopeNode: Element = this.mDocument.createElement("scope");
    // Skip the first scope because that's
    // always the 'current' scope, so we don't want
    // to include it artificially
    for (const packageNamespace of this.mNodeModule.getImportModuleList()) {
      var packageNode: Element = this.mDocument.createElement("package");
      packageNode.setAttribute("id", packageNamespace);
      scopeNode.appendChild(packageNode);
    }
    this.mRoot.appendChild(scopeNode);
  }

  public writeMetadata(metadata: Metadata, parentNode: Element) {
    var metadataNode: Element = this.mDocument.createElement("metadata");
    var count: number = 0;
    for (var ns of metadata.getNamespaces()) {
      var data: Element = this.mDocument.createElement("data");
      data.setAttribute("namespace", ns);
      this.writeGraphNodeData(metadata.getMetadata(ns), data);
      metadataNode.appendChild(data);
      count++;
    }
    if (count > 0) {
      parentNode.appendChild(metadataNode);
    }
  }
  
  public writeDataType(
    id: string,
    dataType: DataType,
    dataTypesNode: Element
  ) {
    var node: Element = this.mDocument.createElement("data-type");
    node.setAttribute("id", dataType.getLocalId());
    node.setAttribute("name", dataType.getName());
    dataTypesNode.appendChild(node);
  }
  
  public writeDataTypes() {
    var dataTypesNode: Element = this.mDocument.createElement("data-types");
    for (const id in this.mNodeModule.getDataTypes()) {
      this.writeDataType(id, this.mNodeModule.getDataTypes()[id], dataTypesNode);
    }
    this.mRoot.appendChild(dataTypesNode);
  }
  
  public writeNamedPorts(
    namedPorts: NamedPorts,
    portsNode: Element
  ) {
    for (var i: number = 0; i < namedPorts.getCount(); i++) {
      const id: string = namedPorts.getName(i);
      const port: NodePort = namedPorts.getByIndex(i)!;
      var portNode: Element = this.mDocument.createElement("port");
      portNode.setAttribute("id", id);
      portNode.setAttribute("description", port.getDescription());
      
      // Un-qualify this if it's a part of our own package,
      var qualifiedDataType: QualifiedIdentifier = new QualifiedIdentifier(port.getDataType());
      var writtenDataType: string;
      if (qualifiedDataType.getNamespace() == this.mNodeModule.getPackage()) {
	writtenDataType = qualifiedDataType.getName();
      }
      else {
	writtenDataType = port.getDataType();
      }
      portNode.setAttribute("data-type", writtenDataType);
      portNode.setAttribute("connection-policy", port.getConnectionPolicy() == NodePort.ConnectionPolicy.Multiple ? "multi" : "one");
      
      this.writeMetadata(port.getMetadata(), portNode);
      
      portsNode.appendChild(portNode);
    }
  }

  public writeNodeType(
    id: string,
    node_type: NodeType,
    nodeTypesNode: Element
  ) {
    var nodeType: Element = this.mDocument.createElement("node-type");
    nodeType.setAttribute("id", node_type.getLocalId());
    if (node_type.getVisibility() == NodeType.Visibility.PUBLIC) {
      nodeType.setAttribute("visibility", "public");
    }
    else {
      nodeType.setAttribute("visibility", "private");
    }
    if (node_type.getType() == NodeType.Type.GRAPH) {
      nodeType.setAttribute("type", "graph");
    }
    else if (node_type.getType() == NodeType.Type.ALIAS) {
      nodeType.setAttribute("type", "alias");
    }
    else {
      nodeType.setAttribute("type", "native");
    }
    
    this.writeMetadata(node_type.getMetadata(), nodeType);
    
    var inputsNode: Element = this.mDocument.createElement("inputs");
    this.writeNamedPorts(node_type.getInputs(), inputsNode);
    nodeType.appendChild(inputsNode);
    
    var outputsNode: Element = this.mDocument.createElement("outputs");
    this.writeNamedPorts(node_type.getOutputs(), outputsNode);
    nodeType.appendChild(outputsNode);
    
    var defaultNodeData: ConnectionData = node_type.getDefaultNodeData();
    if (defaultNodeData.size() != 0) {
      var defaultNodeDataNode: Element = this.mDocument.createElement("default-node-data");
      this.writeGraphNodeData(defaultNodeData, defaultNodeDataNode);
      nodeType.appendChild(defaultNodeDataNode);
    }
    nodeTypesNode.appendChild(nodeType);
  }
  
  public writeNodeTypes() {
    var nodeTypesNode: Element = this.mDocument.createElement("node-types");
    const nodeTypesDict: { [key: string] : NodeType } = this.mNodeModule.getNodeTypes();
    for (var id in nodeTypesDict) {
      var value: NodeType = nodeTypesDict[id];
      this.writeNodeType(id, value, nodeTypesNode);
    }
    this.mRoot.appendChild(nodeTypesNode);
  }

  public writeGraphNodeData(data: ConnectionData, nodeNode: Element) {
    for (var key in data.getData()) {
      var valueNode : Element = this.mDocument.createElement("value");
      valueNode.setAttribute("key", key);
      valueNode.textContent = data.getData()[key];
      
      nodeNode.appendChild(valueNode);
    }
  }


  public writeGraphNode(
    node: Node,
    nodeNode: Element
  ) {

    const nodeType: NodeType = node.getType();
    // If the node type is local to this package, we should omit the package name.
    var qualifiedNodeType: QualifiedIdentifier = new QualifiedIdentifier(nodeType.getFullyQualifiedId());
    var writtenNodeType: string;
    if (qualifiedNodeType.getNamespace() == this.mNodeModule.getPackage()) {
      writtenNodeType = qualifiedNodeType.getName();
    }
    else {
      writtenNodeType = nodeType.getFullyQualifiedId();
    }

    nodeNode.setAttribute("type", writtenNodeType);
    this.writeGraphNodeData(node.getData(), nodeNode);
    
    if (nodeType.getAllowPortOverride()) {
      if (node.hasOverrideInputs()) {
	var inputsNode: Element = this.mDocument.createElement("inputs");
	this.writeNamedPorts(node.getOverrideInputs(), inputsNode);
	nodeNode.appendChild(inputsNode);
      }
      
      if (node.hasOverrideOutputs()) {
	var outputsNode: Element = this.mDocument.createElement("outputs");
	this.writeNamedPorts(node.getOverrideOutputs(), outputsNode);
	nodeNode.appendChild(outputsNode);
      }
    }
    
    this.writeMetadata(node.getMetadata(), nodeNode);
  }


  public writeGraphNodes(
    graph: NodeGraph,
    nodesNode: Element
  ) {
    const graphNodes: { [ key: string ] : Node } = graph.getNodes();
    for (const id in graphNodes) {
      var nodeNode: Element = this.mDocument.createElement("node");
      nodeNode.setAttribute("id", id);
      this.writeGraphNode(graphNodes[id], nodeNode);
      nodesNode.appendChild(nodeNode);
    }
  }
  public writeGraphEdges(
    graph: NodeGraph,
    edgesNode: Element
  ) {
    var edgeMap: { [ key: string ] : Edge } = graph.getEdges();
    for (const id in edgeMap) {
      var edgeNode: Element = this.mDocument.createElement("edge");

      var edge: Edge = edgeMap[id];
      edgeNode.setAttribute("from-node", edge.fromNode);
      edgeNode.setAttribute("from-port", edge.fromPort);
      edgeNode.setAttribute("to-node", edge.toNode);
      edgeNode.setAttribute("to-port", edge.toPort);
      
      edgesNode.appendChild(edgeNode);
    }
  }

  public writeGraph(
    id: string,
    graph: NodeGraph,
    graphsNode: Element) {
    
    var graphNode: Element = this.mDocument.createElement("graph");
    graphNode.setAttribute("id", id);
    
    this.writeMetadata(graph.getMetadata(), graphNode);
    
    var nodesNode: Element = this.mDocument.createElement("nodes");
    this.writeGraphNodes(graph, nodesNode);
    graphNode.appendChild(nodesNode);
    
    var edgesNode: Element = this.mDocument.createElement("edges");
    this.writeGraphEdges(graph, edgesNode);
    graphNode.appendChild(edgesNode);
    
    graphsNode.appendChild(graphNode);
  }
  
  public writeGraphs() {
    var graphsNode: Element = this.mDocument.createElement("graphs");
    var graphsDict: { [key: string] : NodeGraph } = this.mNodeModule.getGraphs();
    for (var id in graphsDict) {
	  this.writeGraph(id, graphsDict[id], graphsNode);
    }
    this.mRoot.appendChild(graphsNode);
  }
  
  public write() : boolean {
    
    this.mRoot.setAttribute("xmlns", SerializerXML.NODEJS_XML_NAMESPACE);

    this.writePackage();
    this.writeScopes();
    this.writeDataTypes();
    this.writeNodeTypes();
    this.writeGraphs();

    this.mDocument.appendChild(this.mRoot);
    
    return true;
  }

}
