from nodejs.ConnectionData import ConnectionData

class Metadata:
    def __init__(self):
        self.mMetadata = {}

    def getMetadata(self, aMetadataNamespace):
        if aMetadataNamespace in self.mMetadata:
            return self.mMetadata[aMetadataNamespace]
        else:
            cd = ConnectionData()
            self.mMetadata[aMetadataNamespace] = cd
            return cd

    def hasMetadata(self, aMetadataNamespace):
        return (aMetadataNamespace in self.mMetadata)

    def getNamespaces(self):
        return list(self.mMetadata.keys())

