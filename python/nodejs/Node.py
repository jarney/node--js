
from nodejs.Metadata import Metadata
from nodejs.NamedPorts import NamedPorts


class Node:
    def __init__(self, aId, aType, aGraph, aData):
        self.mId = aId
        self.mType = aType
        self.mGraph = aGraph
        self.mData = aData.copy()
        self.mMetadata = Metadata()
        self.mOverrideInputs = False
        self.mOverrideOutputs = False
        self.mInputs = NamedPorts()
        self.mOutputs = NamedPorts()

    def getId(self):
        return self.mId

    def getType(self):
        return self.mType

    def getGraph(self):
        return self.mGraph


    def getData(self):
        return self.mData

    def getMetadata(self):
        return self.mMetadata

    def getInputs(self):
        if self.mOverrideInputs:
            return self.mInputs
        else:
            return self.mType.getInputs()

    def getOutputs(self):
        if self.mOverrideOutputs:
            return self.mOutputs
        else:
            return self.mType.getOutputs()
    
    def getOverrideInputs(self):
        return self.mInputs

    def getOverrideOutputs(self):
        return self.mOutputs

    def setOverrideInputs(self, aOverrideInputs):
        self.mOverrideInputs = aOverrideInputs
        
    def setOverrideOutputs(self, aOverrideOutputs):
        self.mOverrideOutputs = aOverrideOutputs

    def hasOverrideInputs(self):
        return self.mOverrideInputs
    
    def hasOverrideOutputs(self):
        return self.mOverrideOutputs
