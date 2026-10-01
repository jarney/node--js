from enum import Enum

class NodeType:
    """
	 * This class represents a type of node in a node program.
	 * Node types are characterized by a unique identifier.
	 * Node types carry a visibility field indicating whether
	 * nodes outside this package are allowed to access it or not.
	 * In addition, a node may be marked as 'native', indicating that
	 * its implementation is provided by the underlying
	 * runtime, or as 'graph' indicating that the implementation
	 * is provided by a graph of other nodes.
	 *
	 * In addition, each node declares input and output ports
	 * which may carry data.  Each input and output port are
	 * associated with a data type.
    """
    
    class Visibility(Enum):
        """
	     * This enum represents the visibility of a node type.
	     * This determines whether graphs outside this package are
	     * permitted to access it.
        """
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

    def setVisibility(self, aVisibility):
        self.mVisibility = aVisibility

    def getType(self):
        return self.mType

    def setType(self, aType):
        self.mType = aType

    def addInputPort(self, aName, aPort):
        if aName in self.mInputsByName:
            return False
        self.mInputsByName[aName] = aPort
        self.mInputs.append(aPort)
        self.mInputNames.append(aName)
        return True

    def getInputPortByName(self, aName):
        if not aName in self.mInputsByName:
            return None
        return self.mInputsByName[aName]

    def getInputPortByIndex(self, aIndex):
        if aIndex >= len(self.mInputs):
            return None
        return self.mInputs[aIndex]
    
    def hasInputPort(self, aName):
        return aName in self.mInputsByName
    
    def getInputPortName(self, aIndex):
        if aIndex >= len(self.mInputNames):
            return ""
        return self.mInputNames[aIndex]
    
    def getInputPortCount(self):
        return len(self.mInputs)

    def addOutputPort(self, aName, aPort):
        if aName in self.mOutputsByName:
            return False
        self.mOutputsByName[aName] = aPort
        self.mOutputs.append(aPort)
        self.mOutputNames.append(aName)
        return True

    def getOutputPortByName(self, aName):
        if not aName in self.mOutputsByName:
            return None
        return self.mOutputsByName[aName]

    def getOutputPortByIndex(self, aIndex):
        if aIndex >= len(self.mOutputs):
            return None
        return self.mOutputs[aIndex]
    
    def hasOutputPort(self, aName):
        return aName in self.mOutputsByName
    
    def getOutputPortName(self, aIndex):
        if aIndex >= len(self.mOutputNames):
            return ""
        return self.mOutputNames[aIndex]
    
    def getOutputPortCount(self):
        return len(self.mOutputs)

