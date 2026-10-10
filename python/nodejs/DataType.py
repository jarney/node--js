"""

"""

class DataType:
    """
    Data types are cool, man.
    """
    def __init__(self, aId, aName):
        self.mId = aId
        self.mName = aName
        pass

    def copy(self, other):
        self.mId = other.mId
        self.mName = other.mName
    
    def getId(self):
        return self.mId
    
    def getName(self):
        return self.mName

    def __eq__(a,b):
        """
        Two data-types are considered equal if they both have
        the same ID.
        """
        if a is None and b is None:
            return True
        if b is None:
            return False
        if a is None:
            return False
        return a.mId == b.mId

    def __ne__(a,b):
        return not (a == b)
