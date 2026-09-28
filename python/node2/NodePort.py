from enum import Enum

class NodePort:
    class ConnectionPolicy:
        One = 0
        Multiple = 1

    def __init__(self):
        self.mType = ""
        self.mDescription = ""
        self.mConnectionPolicy = NodePort.ConnectionPolicy.One

    def setDataType(self, aDataType):
        self.mDataType = aDataType

    def getDataType(self):
        return self.mDataType
        
    def setDescription(self, aDescription):
        self.mDescription = aDescription

    def getDescription(self):
        return self.mDescription

    def setConnectionPolicy(self, aConnectionPolicy):
        self.mConnectionPolicy = aConnectionPolicy

    
