from enum import Enum

class NodeType:

    class Visibility(Enum):
        PUBLIC = 0
        PRIVATE = 1
        
    class Type(Enum):
        NATIVE = 0
        GRAPH = 1
        
    def __init__(self):
        self.mId = ""
        self.mVisibility = ""
        self.mType = ""
        self.mInputsByName = {}
        self.mOutputsByName = {}
        self.mInputs = []
        self.mOutputs = []
        self.mInputNames = []
        self.mOutputNames = []

    def getId(self):
        return self.mId

    def setId(self, id):
        self.mId = id

    def getVisibility(self):
        return self.mVisibility

    def setVisibility(self, visibility):
        self.mVisibility = visibility

    def getType(self):
        return self.mType

    def setType(self, aType):
        self.mType = aType

    def addInputPort(self, aName, aPort):
        self.mInputsByName[aName] = aPort
        self.mInputs.append(aPort)
        self.mInputNames.append(aName)

    def getInputPortByName(self, aName):
        return self.mInputsByName[aName]

    def getInputPortByIndex(self, aIndex):
        return self.mInputs[aIndex]
    
    def hasInputPort(self, aName):
        return aName in self.mInputsByName
    
    def getInputPortName(self, aIndex):
        return aInputNames[aIndex]
    
    def getInputPortCount(self):
        return len(self.mInputs)


    def getOutputPortByName(self, aName):
        return self.mOutputsByName[aName]

    def getOutputPortByIndex(self, aIndex):
        return self.mOutputs[aIndex]
    
    def hasOutputPort(self, aName):
        return aName in self.mOutputsByName
    
    def getOutputPortName(self, aIndex):
        return aOutputNames[aIndex]
    
    def getOutputPortCount(self):
        return len(self.mOutputs)

