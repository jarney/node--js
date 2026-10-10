
from .NodeGraph import NodeGraph

class NodeModule:
    def __init__(self, moduleLoader):
        self.package = ""
        self.mDescription = ""
        self.mModuleLoader = moduleLoader
        self.mDataTypes = {}
        self.mNodeTypes = {}
        self.mGraphs = {}
        self.mMetadata = {}

    def setPackage(self, package):
        """
        Sets the fully-qualified package name.
        """
        self.package = package

    def getPackage(self):
        return self.package

    def setDescription(self, aDescription):
        self.mDescription = aDescription

    def getDescription(self):
        return self.mDescription

    def getModuleLoader(self):
        return self.mModuleLoader

    def addDataType(self, dataType: DataType):
        self.mDataTypes[dataType.getId()] = dataType
        pass

    def removeDataType(self, id):
        del self.mDataTypes[id]

    def getDataTypes(self):
        return self.mDataTypes

    def hasDataType(self, id):
        return (id in self.mDataTypes)

    def getDataType(self, id):
        return self.mDataTypes[id] if id in self.mDataTypes else None

    def addNodeType(self, nodeType: NodeType):
        self.mNodeTypes[nodeType.getId()] = nodeType

    def removeNodeType(self, id):
        del self.mNodeTypes[id]

    def getNodeTypes(self):
        return self.mNodeTypes;

    def hasNodeType(self, id):
        return (id in self.mNodeTypes)

    def getNodeType(self, id):
        return self.mNodeTypes[id] if id in self.mNodeTypes else None

    def addGraph(self, aId):
        # Duplicate graphs are not allowed.
        if aId in self.mGraphs:
            return None

        graph = NodeGraph(self)
        self.mGraphs[aId] = graph
        return graph

    def getGraph(self, aId):
        if not aId in self.mGraphs:
            return None
        return self.mGraphs[aId]

    def getGraphs(self):
        return self.mGraphs

    def getMetadata(self):
        return self.mMetadata

