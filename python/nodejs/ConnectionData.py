

class ConnectionData:
    def __init__(self):
        self.mData = {}

    def copy(self):
        copyData = ConnectionData()
        copyData.mData = self.mData.copy()
        return copyData
        
    def setValue(self, key, value):
        self.mData[key] = value

    def getValue(self, key, default_value = ""):
        if key in self.mData:
            return self.mData[key]
        else:
            return default_value

    def hasValue(self, key):
        return key in self.mData

    def getData(self):
        return self.mData
