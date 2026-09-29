from enum import Enum

class NodePort:
    class ConnectionPolicy:
        One = 0
        Multiple = 1

    def __init__(self, aDataType, aDescription, aConnectionPolicy = ConnectionPolicy.One):
        self.mDataType = aDataType
        self.mDescription = aDescription
        self.mConnectionPolicy = aConnectionPolicy

    def getDataType(self):
        return self.mDataType
        
    def getDescription(self):
        return self.mDescription

    def getConnectionPolicy(self):
        return self.mConnectionPolicy
