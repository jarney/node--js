from .Node import Node
from .Edge import Edge
from .Group import Group
from .Metadata import Metadata

# Useful for states in the
# topological sort algorithm used
# to unwind the dependency order.
TOPOSORT_STATE_TODO = 0
TOPOSORT_STATE_IN_PROGRESS = 1
TOPOSORT_STATE_PROCESSED = 2

class NodeGraph:
    def __init__(self, aModule):
        self.mModule = aModule
        self.mScopes = []
        self.mNodes = {}
        self.mEdges = {}
        self.mEdgesByFromNode = {}
        self.mEdgesByToNode = {}
        self.mGroups = {}
        self.mMetadata = Metadata()

    def findNewNodeId(self, aNodeIdCandidate):
        actualNewId = aNodeIdCandidate
        i = 0
        while (True):
            if actualNewId in self.mNodes:
                actualNewId = aNodeIdCandidate + "-" + str(i)
            else:
                break;
            i += 1
        return actualNewId

    def newNode(self, aNodeType, aNodeIdCandidate, aConnectionData):
        newNodeId = self.findNewNodeId(aNodeIdCandidate)

        retNode = Node(newNodeId, aNodeType, self, aConnectionData)
        self.mNodes[newNodeId] = retNode
        
        return retNode

    def removeNode(self, aNodeId):
        for it in self.mEdgesByFromNode:
            newEdgesByFromNode = []
            for edge in self.mEdgesByFromNode[it]:
                if edge.fromNode == aNodeId or edge.toNode == aNodeId:
                    continue
                newEdgesByFromNode.append(edge)
            self.mEdgesByFromNode[it] = newEdgesByFromNode

        if aNodeId in self.mEdgesByFromNode:
            del self.mEdgesByFromNode[aNodeId]

        for it in self.mEdgesByToNode:
            newEdgesByToNode = []
            for edge in self.mEdgesByToNode[it]:
                if edge.fromNode == aNodeId or edge.toNode == aNodeId:
                    continue
                newEdgesByToNode.append(edge)
            self.mEdgesByToNode[it] = newEdgesByToNode

        if aNodeId in self.mEdgesByToNode:
            del self.mEdgesByToNode[aNodeId]

        edgesToRemove = []
        for edgeId in self.mEdges:
            edge = self.mEdges[edgeId]
            if edge.fromNode == aNodeId or edge.toNode == aNodeId:
                edgesToRemove.append(edgeId)
        
        for edgeId in edgesToRemove:
            del self.mEdges[edgeId]
            
        del self.mNodes[aNodeId]

    def removeEdge(self, aFromNode, aFromPort, aToNode, aToPort):
        tmpEdge = Edge(
            aFromNode, aFromPort,
            aToNode, aToPort
        )
        edgeId = tmpEdge.getId()

        for cnId in self.mEdgesByFromNode:
            newEdgesByFromNode = []
            edgesByFromNode = self.mEdgesByFromNode[cnId]
            for edge in edgesByFromNode:
                if not edge.getId() == edgeId:
                    newEdgesByFromNode.append(edge)
            self.mEdgesByFromNode[cnId] = newEdgesByFromNode

        for cnId in self.mEdgesByToNode:
            newEdgesByToNode = []
            edgesByToNode = self.mEdgesByToNode[cnId]
            for edge in edgesByToNode:
                if not edge.getId() == edgeId:
                    newEdgesByToNode.append(edge)
            self.mEdgesByToNode[cnId] = newEdgesByToNode

        del self.mEdges[edgeId]
        
    def addScope(self, nodeModule):
        self.mScopes.append(nodeModule)

    def copyScope(self, otherGraph):
        self.mScopes = otherGraph.mScopes

    def getScopes(self):
        return self.mScopes

    def getNodeType(self, aTypeName):
        for scope in self.mScopes:
            if scope.hasNodeType(aTypeName):
                return scope.getNodeType(aTypeName)
        return None

    def getDataType(self, aTypeName):
        for scope in self.mScopes:
            if scope.hasDataType(aTypeName):
                return scope.getDataType(aTypeName)
        return None
    
    def getNodeIds(self):
        return self.mNodes.keys()
    
    def getNode(self, aNodeId):
        if aNodeId in self.mNodes:
            return self.mNodes[aNodeId]
        else:
            return None

    def hasNode(self, aNodeId):
        return aNodeId in self.mNodes

    def newEdge(self, aFromNode, aFromPort, aToNode, aToPort):
        
        edge = Edge(
            aFromNode, aFromPort,
            aToNode, aToPort
        )
        
        id = edge.getId();
        if id in self.mEdges:
            return None

        if aFromNode not in self.mEdgesByFromNode:
            self.mEdgesByFromNode[aFromNode] = []
        self.mEdgesByFromNode[aFromNode].append(edge)

        if aToNode not in self.mEdgesByToNode:
            self.mEdgesByToNode[aToNode] = []
        self.mEdgesByToNode[aToNode].append(edge);
        self.mEdges[id] = edge
        return id

    def getEdgesFrom(self, nodeId):
        if not nodeId in self.mEdgesByFromNode:
            return []
        return self.mEdgesByFromNode[nodeId]

    def getEdgesTo(self, nodeId):
        if not nodeId in self.mEdgesByToNode:
            return []
        return self.mEdgesByToNode[nodeId]

    def getEdges(self):
        return self.mEdges
    
    def getNeighborNodeIds(self, aNode):
        neighbors = []

        # No edges means empty neighbor graph
        if not aNode in self.mEdgesByFromNode:
            return neighbors
        
        for edge in self.mEdgesByFromNode[aNode]:
            neighbors.append(edge.toNode)
        neighbors = set(neighbors)
        return neighbors
    
    # This method sorts the nodes in topological
    # order and returns the associated NodeIds
    # This method will return None if the graph
    # has a cycle.
    def getNodeIdsInTopologicalOrder(self):
        state = {node: TOPOSORT_STATE_TODO for node in self.getNodeIds()}
        order = []
    
        for start in self.getNodeIds():
            if state[start] != TOPOSORT_STATE_TODO:
                continue
    
            stack = [(start, False)]
    
            while stack:
                node, processed = stack.pop()
    
                if processed:
                    state[node] = TOPOSORT_STATE_PROCESSED
                    order.append(node)
                    continue
    
                if state[node] == TOPOSORT_STATE_PROCESSED:
                    continue
    
                if state[node] == TOPOSORT_STATE_IN_PROGRESS:
                    return None
    
                state[node] = TOPOSORT_STATE_IN_PROGRESS
                stack.append((node, True))
    
                for neighbor in self.getNeighborNodeIds(node):
                    if state[neighbor] == TOPOSORT_STATE_IN_PROGRESS:
                        return None
                    if state[neighbor] == TOPOSORT_STATE_TODO:
                        stack.append((neighbor, False))
        return order

    def getModule(self):
        return self.mModule

    def getGroups(self):
        return self.mGroups

    def addGroup(self, aGroupId):
        self.mGroups[aGroupId] = Group()

    def removeGroup(self, aGroupId):
        if aGroupId in self.mGroups:
            del self.mGroups[aGroupId]

    def hasGroup(self, aGroupId):
        return aGroupId in self.mGroups


    def getGroup(self, aGroupId):
        return self.mGroups if aGroupId in self.mGroups else None

    def getMetadata(self):
        return self.mMetadata
    
    
