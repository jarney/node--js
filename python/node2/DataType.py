

class DataType:
    def __init__(self, id, name):
        self.id = id
        self.name = name
        pass

    def getId(self):
        return self.id
    
    def getName(self):
        return self.name

    def print(self):
        print("id: " + self.id + " name: " + self.name)

