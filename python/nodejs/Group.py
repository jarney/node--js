

class Group:
    def __init__(self):
        self.mNodes = {}

    def addNode(self, aNodeId):
        self.mNodes[aNodeId] = aNodeId

    def removeNode(self, aNodeId):
        if (aNodeId in self.mNodes):
            del self.mNodes[aNodeId]

    def getNodes(self):
        return self.mNodes.keys()

    def contains(self, aNodeId):
        return aNodeId in self.mNodes
