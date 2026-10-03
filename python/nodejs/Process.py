from nodejs.ConnectionData import ConnectionData
from nodejs.NodeType import NodeType

class Processor:
    def __init__(self):
        self.native_impl = {}

    def setNativeImpl(self, nodeTypeId, fn):
        self.native_impl[nodeTypeId] = fn
        
    def processGraph(self, graph):
        # Being in topological order ensures that each node
        # will have its prececessors calculated first
        # so it will have the input data it needs by the time
        # it is called.
        nodeData = {}
        
        nodeIds = graph.getNodeIdsInTopologicalOrder()
        nodeData[nodeIds[0]] = ConnectionData()

        lastData = None
        for nodeId in nodeIds:
            node = graph.getNode(nodeId)

            # Get the type of the node.
            nodeType = node.getType()

            # When processing a node,
            # we're really getting all of the data from
            # prececessor nodes, so we need to shuttle it
            # to the right place.
            fromData = ConnectionData()
            for edge in graph.getEdgesFrom(nodeId):
                fromData.setValue(edge.fromPort, nodeData[edge.toNode].getValue(edge.toPort))
            toData = ConnectionData()
            self.processNodeType(graph, nodeType, node, fromData, toData)
            nodeData[nodeId] = toData.copy()
            lastData = toData
            
        return lastData

    def processNodeType(self, graph, nodeType, node, fromData, toData):
        if nodeType.getType() == NodeType.Type.NATIVE:
            nodeTypeId = nodeType.getId()
            if nodeTypeId not in self.native_impl:
                self.defaultProcess(node, fromData, toData)
            else:
                processor = self.native_impl[nodeTypeId]
                processor(node, fromData, toData)
        elif nodeType.getType() == NodeType.Type.GRAPH:
            module = graph.getModule()
            subgraph = module.getGraph(nodeType.getId())
            self.processGraph(subgraph)
        
    def defaultProcess(self, node, fromData, toData):
        print("Processing node " + node.getType().getId())
