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
    def __init__(self, aId, aName):
        self.mId = aId
        self.mName = aName
        pass

    def getId(self):
        return self.mId
    
    def getName(self):
        return self.mName

    def __eq__(a,b):
        """
        Two data-types are considered equal if they both have
        the same ID.
        """
        return a.mId == b.mId
