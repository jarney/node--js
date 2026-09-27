

class NodeModule:
    def __init__(self):
        self.dataTypes = {}
        self.nodeTypes = {}

    def addDataType(self, dataType: DataType):
        self.dataTypes[dataType.getId()] = dataType
        pass

    def removeDataType(self, id):
        del self.dataTypes[id]

    def getDataTypes(self):
        return self.dataTypes

    def hasDataType(self, id):
        return (id in self.dataTypes)

    def getDataType(self, id):
        return self.dataTypes[id]
