import type {NodeId, GroupId, EdgeId, PortId, NodeTypeId, DataTypeId} from "./Definitions.ts";
import {Node} from "./Node.ts";
import {DataType} from "./DataType.ts";
import {NodeType} from "./NodeType.ts";
import {Group} from "./Group.ts";
import {Edge} from "./Edge.ts";
import {ConnectionData} from "./ConnectionData.ts";
import {NodeModule} from "./NodeModule.ts";
import {Metadata} from "./Metadata.ts";

class StackEntry {
  public nodeId: NodeId;
  public processed: boolean;
  
  constructor(aNodeId: NodeId, aProcessed: boolean) {
    this.nodeId = aNodeId;
    this.processed = aProcessed;
  }
}
  
export class NodeGraph {
  mModule: NodeModule;
  mNodes: { [ key: NodeId ] : Node };
  mGroups: { [ key: GroupId ] : Group };
  mEdges: { [ key: EdgeId ] : Edge };
  mEdgesByFromNode: { [key: NodeId] : Edge[] };
  mEdgesByToNode: { [key: EdgeId] : Edge[] };
  mScopes: NodeModule[];
  mMetadata: Metadata;
  
  constructor(aModule: NodeModule) {
    this.mModule = aModule;
    this.mNodes = {};
    this.mGroups = {};
    this.mEdges = {};
    this.mEdgesByFromNode = {};
    this.mEdgesByToNode = {};
    this.mScopes = [];
    this.mMetadata = new Metadata();
  }

  public getModule() : NodeModule {
    return this.mModule;
  }

  private findNewNodeId(aNodeIdCandidate: NodeId) : NodeId {
    var actualNewId: NodeId = aNodeIdCandidate;
    var i: number = 0;
    while (actualNewId in this.mNodes) {
      actualNewId = aNodeIdCandidate + "-" + i;
      i++;
    }
    return actualNewId;
  }
  
  public newNode(
    aNodeType: NodeType,
    aNodeIdCandidate: NodeId,
    aConnectionData: ConnectionData = new ConnectionData()
  ) : Node {
    var newNodeId: NodeId = this.findNewNodeId(aNodeIdCandidate);

    var newNode = new Node(newNodeId, aNodeType, this, aConnectionData);
    this.mNodes[newNodeId] = newNode;
    return newNode;
  }
  public getNode(aNodeId: NodeId) : Node | undefined {
    return this.mNodes[aNodeId];
  }
  public hasNode(aNodeId: NodeId) : boolean {
    return (aNodeId in this.mNodes);
  }
  public removeNode(aNodeId: NodeId) : void {
    for (var nodeId in this.mEdgesByFromNode) {
      if (nodeId in this.mEdgesByFromNode) {
	var newEdgesByFromNode: Edge[] = [];
	var edgesByFromNode: Edge[] = this.mEdgesByFromNode[nodeId]!;
	for (var edgeIndex in edgesByFromNode) {
	  var edge: Edge = edgesByFromNode[edgeIndex]!;
          if (edge.fromNode == aNodeId || edge.toNode == aNodeId) {
            continue;
          }
          newEdgesByFromNode.push(edge);
	}
	this.mEdgesByFromNode[nodeId] = newEdgesByFromNode
      }
    }
    if (aNodeId in this.mEdgesByFromNode) {
      delete this.mEdgesByFromNode[aNodeId];
    };
    for (var nodeId in this.mEdgesByToNode) {
      if (nodeId in this.mEdgesByToNode) {
	var newEdgesByToNode: Edge[] = [];
	var edgesByToNode: Edge[] = this.mEdgesByToNode[nodeId]!;
	for (var edgeIndex in edgesByToNode) {
	  var edge: Edge = edgesByToNode[edgeIndex]!;
          if (edge.fromNode == aNodeId || edge.toNode == aNodeId) {
            continue;
          }
          newEdgesByToNode.push(edge);
	}
	this.mEdgesByToNode[nodeId] = newEdgesByToNode
      }
    }
    if (aNodeId in this.mEdgesByToNode) {
      delete this.mEdgesByToNode[aNodeId];
    };
    var edgesToRemove: EdgeId[] = [];
    for (var edgeId in this.mEdges) {
      var edge: Edge = this.mEdges[edgeId]!;
      if (edge.fromNode == aNodeId || edge.toNode == aNodeId) {
        edgesToRemove.push(edgeId);
      }
      for (edgeIndex in edgesToRemove) {
	var edgeId = edgesToRemove[edgeIndex]!;
        delete this.mEdges[edgeId];
      }
    }
    delete this.mNodes[aNodeId];
  }
    
  public getNodes() : { [ key: string ] : Node } {
    return this.mNodes;
  }
  public newEdge(
    aFromNode: NodeId,
    aFromPort: PortId,
    aToNode: NodeId,
    aToPort: PortId
  ) : EdgeId | undefined {
    var connection: Edge = new Edge(
      aFromNode, aFromPort,
      aToNode, aToPort
    );
    var id: EdgeId = connection.getId();
    if (id in this.mEdges) {
      return undefined;
    }

    if (aFromNode in this.mEdgesByFromNode) {
      this.mEdgesByFromNode[aFromNode]!.push(connection);
    }
    else {
      this.mEdgesByFromNode[aFromNode] = [ connection ];
    }
    if (aToNode in this.mEdgesByToNode) {
      this.mEdgesByToNode[aToNode]!.push(connection);
    }
    else {
      this.mEdgesByToNode[aToNode] = [ connection ];
    }
    this.mEdges[id] = connection;
    return id;
  }
  
  public removeEdge(
    aFromNode: NodeId,
    aFromPort: PortId,
    aToNode: NodeId,
    aToPort: PortId
  ) : void {
    var tmpEdge = new Edge(
      aFromNode, aFromPort,
      aToNode, aToPort
    );
    var id: EdgeId = tmpEdge.getId();

    if (aFromNode in this.mEdgesByFromNode) {
      var edgesByFromNode: Edge[] = this.mEdgesByFromNode[aFromNode]!;
      var newEdgesByFromNode: Edge[] = []
      for (var edgeIndex in edgesByFromNode) {
	var edge: Edge = edgesByFromNode[edgeIndex]!;
	if (edge.getId() != id) {
	  newEdgesByFromNode.push(edge);
	}
      }
      this.mEdgesByFromNode[aFromNode] = newEdgesByFromNode;
    }

    if (aToNode in this.mEdgesByToNode) {
      var edgesByToNode: Edge[] = this.mEdgesByToNode[aToNode]!;
      var newEdgesByToNode: Edge[] = []
      for (var edgeIndex in edgesByToNode) {
	var edge: Edge = edgesByToNode[edgeIndex]!;
	if (edge.getId() != id) {
	  newEdgesByToNode.push(edge);
	}
      }
      this.mEdgesByToNode[aToNode] = newEdgesByToNode;
    }
    delete this.mEdges[id];
  }
  private static emptyEdges: Edge[] = [];
  
  public getEdgesFrom(aNodeId: NodeId) : Edge[] {
    if (aNodeId in this.mEdgesByFromNode) {
      return this.mEdgesByFromNode[aNodeId]!;
    }
    else {
      return NodeGraph.emptyEdges;
    }
  }
  public getEdgesTo(aNodeId: NodeId) : Edge[] {
    if (aNodeId in this.mEdgesByToNode) {
      return this.mEdgesByToNode[aNodeId]!;
    }
    else {
      return NodeGraph.emptyEdges;
    }
  }
  public getEdges() : { [ key: EdgeId ] : Edge } {
    return this.mEdges;
  }
  public getScopes() : NodeModule[] {
    return this.mScopes;
  }
  public addScope(aNodeModule : NodeModule ) : void {
    this.mScopes.push(aNodeModule);
  }
  public copyScope(aOther: NodeGraph) : void {
    this.mScopes = [];
    for (var scopeIndex in aOther.mScopes) {
      this.mScopes.push(aOther.mScopes[scopeIndex]!);
    }
  }
  public getNodeType(aNodeTypeId: NodeTypeId) : NodeType | undefined {
    for (var scopeIndex in this.mScopes) {
      var scope : NodeModule = this.mScopes[scopeIndex]!;
      if (scope.hasNodeType(aNodeTypeId)) {
	return scope.getNodeType(aNodeTypeId);
      }
    }
    return undefined;
  }
  public getDataType(aDataTypeId: DataTypeId) : DataType | undefined {
    for (var scopeIndex in this.mScopes) {
      var scope : NodeModule = this.mScopes[scopeIndex]!;
      if (scope.hasDataType(aDataTypeId)) {
	return scope.getDataType(aDataTypeId);;
      }
    }
    return undefined;    
  }

  public getNodeIds() : NodeId[] {
    return Object.keys(this.mNodes);
  }

  public getNeighborNodeIds(aNodeId: NodeId) : NodeId[] {
    var neighbors : NodeId[] = [];
    var neighborMap : { [ key: NodeId ] : NodeId } = {};

    // No edges means empty neighbor graph
    if (!(aNodeId in this.mEdgesByFromNode)) {
	return neighbors;
    }
    var edges: Edge[] = this.mEdgesByFromNode[aNodeId]!;
    for (var edgeIndex in edges) {
      var edge: Edge = edges[edgeIndex]!;
      if (edge.toNode in neighborMap) continue;
      neighbors.push(edge.toNode);
      neighborMap[edge.toNode] = edge.toNode;
    }

    return neighbors;
  }
  

  // Useful for states in the
  // topological sort algorithm used
  // to unwind the dependency order.
  private static TOPOSORT_STATE_TODO: number = 0;
  private static TOPOSORT_STATE_IN_PROGRESS: number = 1;
  private static TOPOSORT_STATE_PROCESSED: number = 2;

  public getNodeIdsInTopologicalOrder() : NodeId[] | undefined {
    var state: { [ key: NodeId ] : number } = {};
    var nodeIds: NodeId[] = this.getNodeIds();
    for (var nodeIndex in nodeIds) {
      var nodeId: NodeId = nodeIds[nodeIndex]!;
      state[nodeId] = NodeGraph.TOPOSORT_STATE_TODO;
    }

    var order: NodeId[] = [];

    for (var startIndex in nodeIds) {
      var start: NodeId = nodeIds[startIndex]!;
      if (state[start] != NodeGraph.TOPOSORT_STATE_TODO) {
	continue;
      }
      var stack: StackEntry[] = [];
      stack.push(new StackEntry(start, false));

      while (stack.length > 0) {
	var pair: StackEntry = stack.pop()!;

	var node: NodeId = pair.nodeId;
	var processed: boolean = pair.processed;

	if (processed) {
	  state[node] = NodeGraph.TOPOSORT_STATE_PROCESSED;
	  order.push(node);
	  continue;
	}
	if (state[node] == NodeGraph.TOPOSORT_STATE_PROCESSED) {
	  continue;
	}
	if (state[node] == NodeGraph.TOPOSORT_STATE_IN_PROGRESS) {
	  // Graph contains a cycle.
	  return undefined;
	}
	
	state[node] = NodeGraph.TOPOSORT_STATE_IN_PROGRESS;
	stack.push(new StackEntry(node, true));

	var neighborNodeIds: NodeId[] = this.getNeighborNodeIds(node);
	for ( var neighborNodeIndex in neighborNodeIds) {
	  var neighborNodeId = neighborNodeIds[neighborNodeIndex]!;
	  if (state[neighborNodeId] == NodeGraph.TOPOSORT_STATE_IN_PROGRESS) {
	    // Graph contains a cycle.
	    return undefined;
	  }
	  if (state[neighborNodeId] == NodeGraph.TOPOSORT_STATE_TODO) {
	    var newEntry: StackEntry = new StackEntry(neighborNodeId, false);
	    stack.push(newEntry);
	  }
	}
      }
    }
    order = order.reverse();
    return order;
  }

  public getGroups() : { [ key: GroupId ] : Group } {
    return this.mGroups;
  }
  public addGroup(aGroupId: GroupId) : void {
    this.mGroups[aGroupId] = new Group();
  }
  public removeGroup(aGroupId: GroupId) : void {
    delete this.mGroups[aGroupId];
  }

  public hasGroup(aGroupId: GroupId) : boolean {
    return (aGroupId in this.mGroups);
  }
  public getGroup(aGroupId: GroupId) : Group | undefined {
    return this.mGroups[aGroupId];
  }
  
  public getMetadata() : Metadata {
    return this.mMetadata;
  }
}
