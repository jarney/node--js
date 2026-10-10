from enum import Enum
from nodejs.Metadata import Metadata

class NodePort:
    class ConnectionPolicy:
        One = 0
        Multiple = 1

    def __init__(self, aDataType, aDescription, aConnectionPolicy = ConnectionPolicy.One):
        self.mDataType = aDataType
        self.mDescription = aDescription
        self.mConnectionPolicy = aConnectionPolicy
        self.mMetadata = Metadata()

    def getDataType(self):
        return self.mDataType
        
    def getDescription(self):
        return self.mDescription

    def getConnectionPolicy(self):
        return self.mConnectionPolicy

    def getMetadata(self):
        return self.mMetadata
