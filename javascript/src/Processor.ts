import type {NodeId, NodeTypeId} from "./Definitions";
import {Node} from "./Node";
import {NodeType} from "./NodeType";
import {NodeModule} from "./NodeModule";
import {Edge} from "./Edge";
import {ConnectionData} from "./ConnectionData";
import {NodeGraph} from "./NodeGraph";


function dumpDict(msg: string, data: ConnectionData) : void {
  console.log(msg, data);
}

/**
 * A node processor takes the information
 * from a node and the input data and calculates
 * a result into the 'toData'.
 */
export interface NodeProcessor {
  process(
    processor: Processor,
    node: Node,
    fromData: ConnectionData,
    toData: ConnectionData
  ) : void;
}

export class Processor {
  mNodeProcessors: { [ key: NodeTypeId ] : NodeProcessor };
  constructor() {
    this.mNodeProcessors = {}
  }
  setNativeImpl(aNodeTypeId: NodeTypeId, nodeProcessor: NodeProcessor) : void {
    this.mNodeProcessors[aNodeTypeId] = nodeProcessor;
  }

  processGraph(
    graph: NodeGraph,
    input: ConnectionData,
    output: ConnectionData) : void {
    // Being in topological order ensures that each node
    // will have its prececessors calculated first
    // so it will have the input data it needs by the time
    // it is called.
    var nodeData: { [ key: string ] : ConnectionData } = {};
        
    var maybeNodeIds: NodeId[] | undefined = graph.getNodeIdsInTopologicalOrder();
    if (maybeNodeIds == undefined) {
      console.log("@@@ CYCLE DETECTED @@@");
      return;
    }
    var nodeIds: NodeId[] = maybeNodeIds!;

    if (nodeIds.length == 0) {
      console.log("### EMPTY GRAPH ###");
      return;
    }

    var lastData: ConnectionData = new ConnectionData();

    nodeData[nodeIds[0]!] = input;

    for (var nodeIndex in nodeIds) {
      var nodeId = nodeIds[nodeIndex]!;
      //console.log("Processing node " + nodeId);
      var node: Node = graph.getNode(nodeId)!;

      // Get the type of the node.
      var nodeType: NodeType = node.getType();
      
      // When processing a node,
      // we're really getting all of the data from
      // prececessor nodes, so we need to shuttle it
      // to the right place.
      var fromData: ConnectionData = new ConnectionData();
      var edgesTo: Edge[] = graph.getEdgesTo(nodeId);
      for (var edgeIndex in edgesTo) {
	var edge: Edge = edgesTo[edgeIndex]!;
	var data: string = nodeData[edge.fromNode]!.getValue(edge.fromPort);
        fromData.appendValue(edge.toPort, data);
	//console.log("Taking data from " +
	//  edge.fromNode + ":" + edge.fromPort + " . " +
	//  edge.toNode + ":" + edge.toPort
	//);

      }
      var toData: ConnectionData = new ConnectionData();
      this.processNodeType(graph, nodeType, node, fromData, toData);
      nodeData[nodeId] = toData;
      lastData = toData;
    }
    output.copy(lastData);
    //dumpDict("Graph output:", output);
  }
  
  processNodeType(
    graph: NodeGraph,
    nodeType: NodeType,
    node: Node,
    fromData: ConnectionData,
    toData: ConnectionData
  ) : void {
    if (nodeType.getType() == NodeType.Type.NATIVE) {
      var nodeTypeId: NodeTypeId = nodeType.getFullyQualifiedId();
      if (!(nodeTypeId in this.mNodeProcessors)) {
	Processor.default_processor(this, node, fromData, toData);
	return;
      }
      //console.log("Processing node type " + nodeTypeId);
      //dumpDict("input:", fromData);
      var processor: NodeProcessor = this.mNodeProcessors[nodeTypeId]!;
      processor.process(this, node, fromData, toData);
      //dumpDict("output: ", toData);
    }
    else if (nodeType.getType() == NodeType.Type.GRAPH) {
      var module: NodeModule = graph.getModule();
      var subgraph: NodeGraph | undefined = module.getGraph(nodeType.getFullyQualifiedId());
      if (subgraph == undefined) {
	Processor.default_processor(this, node, fromData, toData);
	return;
      }
      // Rules:
      // - Only one terminal node called "output".
      // - Each output corresponds to the node type's output ports.
      // - Each input corresponds to the node type's input ports.
      // All of this implies:
      // - Assignment must produce a value (an assignment).
      // - Module Instantiation must be a value by itself (this is 'geometry')
      // - Function definition is also its own thing producing a value.
      // - Module definition also produces its own thing.
      this.processGraph(subgraph!, fromData, toData);
    }
  }

  
  static default_processor(
    processor: Processor,
    node: Node,
    input: ConnectionData,
    output: ConnectionData
  ) : void {
    console.log("Processor: default_processor");
  }
}


