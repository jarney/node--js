import type {NodeId, GroupId, EdgeId} from "./Definitions.ts";
import {Node} from "./Node.ts";
import {NodeModule} from "./NodeModule.ts";
import {Metadata} from "./Metadata.ts";

export class NodeGraph {
  mModule: NodeModule;
  mNodes: { [ key: NodeId ] : Node };
  mGroups: { [ key: GroupId ] : Group };
  mEdges: { [ key: EdgeId ] : Edge };
  mEdgesByFromNode: { [key: NodeId] : Edge[] };
  mEdgesByToNode: { [key: EdgeId] : Edge[] };
  mScopes: NodeModule[];
  mMetdata: Metadata;
  
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

  public getModule() : NodeModules {
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
      var newEdgesByFromNode: Edge[] = [];
      for (var edge in this.mEdgesByFromNode[nodeId]) {
        if (edge.fromNode == aNodeId || edge.toNode == aNodeId) {
          continue;
        }
        newEdgesByFromNode.append(edge);
      }
      this.mEdgesByFromNode[inodeId] = newEdgesByFromNode
    }
    if (aNodeId in this.mEdgesByFromNode) {
      delete this.mEdgesByFromNode[aNodeId];
    };

    for (var nodeId in this.mEdgesByToNode) {
      var newEdgesByToNode: Edge[] = [];
      for (var edge in this.mEdgesByToNode[nodeId]) {
        if (edge.fromNode == aNodeId || edge.toNode == aNodeId) {
          continue;
        }
        newEdgesByToNode.append(edge);
      }
      this.mEdgesByToNode[inodeId] = newEdgesByToNode
    }
    if (aNodeId in this.mEdgesByToNode) {
      delete this.mEdgesByToNode[aNodeId];
    };
    
    var edgesToRemove: EdgeId[] = [];
    for (var edgeId in this.mEdges) {
      var edge: Edge = this.mEdges[edgeId];
      if (edge.fromNode == aNodeId || edge.toNode == aNodeId) {
        edgesToRemove.append(edgeId);
      }
      for (edgeId in edgesToRemove) {
        delete this.mEdges[edgeId];
      }
    }
    delete this.mNodes[aNodeId]
  }
    
  public getNodes() : { [ key: string ] : Node } {
    return this.mNodes;
  }
  public newEdge(
    aFromNode: NodeId,
    aFromPort: PortId,
    aToNode: NodeId,
    aToPort: PortId
  ) : Node | undefined {
    var connection: Edge = new Edge(
      aFromNode, aFromPort,
      aToNode, aToPort
    );
    var id: EdgeId = connection.getId();
    if (id in this.mEdges) {
      return undefined;
    }

    if (aFromNode in this.mEdgesByFromNode) {
      this.mEdgesByFromNode[aFromNode].push(connection);
    }
    else {
      this.mEdgesByFromNode[aFromNode] = [ connection ];
    }
    if (aToNode in this.mEdgesByToNode) {
      this.mEdgesByToNode[aToNode].push(connection);
    }
    else {
      this.mEdgesByToNode[aToNode] = [ connection ];
    }
    this.mEdges[id] = connection;
    return id;
  }
  
/*	    
	    void removeEdge(
		NodeId aFromNode,
		PortId aFromPort,
		NodeId aToNode,
		PortId aToPort
		);

	    const std::vector<const Edge *> & getEdgesFrom(NodeId aNodeId) const;

	    const std::vector<const Edge*> & getEdgesTo(NodeId aNodeId) const;

	    const std::map<EdgeId, std::unique_ptr<Edge>> & getEdges() const;

	    const std::vector<const NodeModule *> & getScopes() const;

	    void addScope(const NodeModule * node_module);

	    void copyScope(const NodeGraph *other);
	    
	    const NodeType * getNodeType(std::string aTypeName);
	    const DataType * getDataType(std::string aTypeName);

	    std::set<NodeId> getNodeIds(void) const;

	    std::set<NodeId> getNeighborNodeIds(NodeId aNode) const;

	    std::optional<std::vector<NodeId>> getNodeIdsInTopologicalOrder(void) const;
	    
 */
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
