from .Node import Node
from .Connection import Connection

class NodeGraph:
    def __init__(self):
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

    def getNode(self, aNodeId):
        if aNodeId in self.mNodes:
            return self.mNodes[aNodeId]
        else:
            return None

    def hasNode(self, aNodeId):
        return aNodeId in self.mNodes

    def newEdge(self, aFromNode, aFromPort, aToNode, aToPort):
        
        connection = Connection(
            aFromNode, aFromPort,
            aToNode, aToPort
        )
        
        id = connection.getId();
        if id in self.mEdges:
            return None

        if aFromNode not in self.mConnectionsByFromNode:
            self.mConnectionsByFromNode[aFromNode] = []
        self.mConnectionsByFromNode[aFromNode].append(connection)

        if aToNode not in self.mConnectionsByToNode:
            self.mConnectionsByToNode[aToNode] = []
        self.mConnectionsByToNode[aToNode].append(connection);
        self.mEdges[id] = connection
        return id

    
