
function NodeModule() {
    this.mPackage = "";
    this.mDataTypes = {};
    this.mNodeTypes = {};
};


NodeModule.prototype.setPackage = function(aPackage) {
    this.mPackage = aPackage;
};

NodeModule.prototype.getPackage = function() {
    return this.mPackage;
};

NodeModule.prototype.addDataType = function(aDataType) {
    if (aDataType.getId() in self.mDataTypes) return false;
    self.mDataTypes[dataType.getId()] = aDataType;
};

NodeModule.prototype.removeDataType = function(aId) {
    delete self.mDataTypes[aId];
};

/*
    def getDataTypes(self):
        return self.dataTypes

    def hasDataType(self, id):
        return (id in self.dataTypes)

    def getDataType(self, id):
        return self.dataTypes[id]

    def addNodeType(self, nodeType: NodeType):
        self.nodeTypes[nodeType.getId()] = nodeType

    def getNodeTypes(self):
        return self.nodeTypes;

    def hasNodeType(self, id):
        return (id in self.nodeTypes)

    def getNodeType(self, id):
        return self.nodeTypes[id]
    
*/
