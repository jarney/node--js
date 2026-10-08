

class NamedPorts:
    def __init__(self):
        self.mPortsByName = {}
        self.mPorts = []
        self.mPortNames = []
        self.mPortIndices = {}

    def addPort(self, name, port):
        if name in self.mPortsByName:
            return False
    
        self.mPortsByName[name] = port
        self.mPorts.append(port)
        self.mPortNames.append(name)
        self.mPortIndices[name] = len(self.mPorts) - 1
        return True
    

    def getByName(self, name):
        return self.mPortsByName[name] if name in self.mPortsByName else None

    def getByIndex(self, index):
        return self.mPorts[index] if index < len(self.mPorts) else None

    def hasPort(self, name):
        return name in self.mPortsByName

    def getName(self, index):
        return self.mPortNames[index] if index < len(self.mPortNames) else ""

    def getPortIndex(self, name):
        return self.mPortIndices[name] if (name in self.mPortIndices) else -1

    def getCount(self):
        return len(self.mPorts)
