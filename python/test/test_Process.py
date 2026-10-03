import pytest

from nodejs.NodeGraph import NodeGraph
from nodejs.NodeType import NodeType
from nodejs.ConnectionData import ConnectionData
from nodejs.Process import Processor

def binary_processor(node, fromData, toData):
    toData.setValue("out", "binary call node(" + fromData.getValue("in1") + ", " + fromData.getValue("in2") + ")")
    return

def unary_processor(node, fromData, toData):
    toData.setValue("out", "callDataFrom(" + node.getId() + ")")
    return

def test_NodeGraph_create_node():
    graph = NodeGraph()
    nodeData = ConnectionData()

    unaryType = NodeType()
    unaryType.setId("unary")
    unaryType.setType(NodeType.Type.NATIVE)

    binType = NodeType()
    binType.setId("binary")
    binType.setType(NodeType.Type.NATIVE)
    
    main = graph.newNode(binType, "main", nodeData)
    call1 = graph.newNode(unaryType, "call1", nodeData)
    call2 = graph.newNode(unaryType, "call2", nodeData)

    graph.newEdge("main", "in1", "call1", "out")
    graph.newEdge("main", "in2", "call2", "out")

    processor = Processor()
    processor.setNativeImpl("unary", unary_processor)
    processor.setNativeImpl("binary", binary_processor)
    
    out = processor.processGraph(graph)
    print(out.getData())
