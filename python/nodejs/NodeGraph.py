from .Node import Node
from .Edge import Edge

# Useful for states in the
# topological sort algorithm used
# to unwind the dependency order.
TOPOSORT_STATE_TODO = 0
TOPOSORT_STATE_IN_PROGRESS = 1
TOPOSORT_STATE_PROCESSED = 2

class NodeGraph:
    def __init__(self, aModule):
        self.mModule = aModule
        self.mNodes = {}
        self.mEdges = {}
        self.mEdgesByFromNode = {}
        self.mEdgesByToNode = {}

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
