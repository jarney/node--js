from enum import Enum

from .NamedPorts import NamedPorts
from .ConnectionData import ConnectionData
from .Metadata import Metadata

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
        self.mVisibility = NodeType.Visibility.PUBLIC
        self.mAllowPortOverride = False
        self.mType = NodeType.Type.GRAPH
        self.mInputs = NamedPorts()
        self.mOutputs = NamedPorts()
        self.mDefaultNodeData = ConnectionData()
        self.mMetadata = Metadata()

    def getId(self):
        return self.mId

    def setId(self, id):
        self.mId = id

    def getVisibility(self):
        return self.mVisibility

    def setVisibility(self, aVisibility):
        self.mVisibility = aVisibility

    def getAllowPortOverride(self):
        return self.mAllowPortOverride

    def setAllowPortOverride(self, aAllowPortOverride):
        self.mAllowPortOverride = aAllowPortOverride
            
    def getType(self):
        return self.mType

    def setType(self, aType):
        self.mType = aType

    def getInputs(self):
        return self.mInputs

    def getOutputs(self):
        return self.mOutputs

    def getDefaultNodeData(self):
        return self.mDefaultNodeData

    def setDefaultNodeData(self, aConnectionData):
        self.mDefaultNodeData = aConnectionData

    def getMetadata(self):
        return self.mMetadata
