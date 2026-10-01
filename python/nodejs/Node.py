
class Node:
    def __init__(self, aId, aType, aGraph, aData):
        self.mId = aId
        self.mType = aType
        self.mGraph = aGraph
        self.mData = aData.copy()

    def getId(self):
        return self.mId

    def getType(self):
        return self.mType

    def getGraph(self):
        return self.mGraph

    def getData(self):
        return self.mData
