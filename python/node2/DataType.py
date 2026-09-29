"""
This example module shows various types of documentation available for use
with pydoc.  To generate HTML documentation for this module issue the
command:

    pydoc -w foo

"""

class DataType:
    """
    Data types are cool, man.
    """
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

