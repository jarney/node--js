

class Connection:
    def __init__(self, aFromNode, aFromPort, aToNode, aToPort):
        self.fromNode = aFromNode
        self.fromPort = aFromPort
        self.toNode = aToNode
        self.toPort = aToPort

    def getId(self):
        return self.fromNode + "-" + self.fromPort + \
            "|" + \
            self.toNode + "-" + self.toPort

    def __eq__(a,b):
        """
        Two data-types are considered equal if they both have
        the same ID.
        """
        return (a.fromNode == b.fromNode) and \
            (a.fromPort == b.fromPort) and \
            (a.toNode == b.toNode) and \
            (a.toPort == b.toPort)
    
    def __ne__(a,b):
        return not (a == b)
