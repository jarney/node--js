import pytest

from nodejs.NodeGraph import NodeGraph
from nodejs.NodeType import NodeType
from nodejs.ConnectionData import ConnectionData

def test_NodeGraph_create_node():
    graph = NodeGraph()
    nodeData = ConnectionData()
    nodeType = NodeType()

    created = graph.newNode(nodeType, "main", nodeData)
    assert(created.getId() == "main")
    
    nextNode0 = graph.newNode(nodeType, "main", nodeData)
    assert(nextNode0.getId() == "main-0")

    nextNode1 = graph.newNode(nodeType, "main", nodeData)
    assert(nextNode1.getId() == "main-1")

def test_NodeGraph_registered_node_type():
    graph = NodeGraph()
    nodeData = ConnectionData()
    nodeType = NodeType()

    createdNode = graph.newNode(nodeType, "main", nodeData)
    assert(createdNode.getType() == nodeType)
    assert(createdNode.getGraph() == graph)
    

def test_NodeGraph_copied_data():
    graph = NodeGraph()
    nodeData = ConnectionData()
    nodeType = NodeType()

    nodeData.setValue("initial-value", "some-data")
    createdNode = graph.newNode(nodeType, "main", nodeData)

    # Check that our node's data was initialized with the data
    # we gave it.
    assert(createdNode.getData().hasValue("initial-value"))

    # Check that our node's data is a copy and doesn't
    # refer to the same data.
    createdNode.getData().setValue("runtime-value", "another one")
    assert(createdNode.getData().hasValue("runtime-value"))
    assert(not nodeData.hasValue("runtime-value"))

    nodeData.setValue("runtime-for-initializer", "initializer")
    assert(not createdNode.getData().hasValue("runtime-for-initializer"))
    assert(nodeData.hasValue("runtime-for-initializer"))


def test_NodeGraph_node_existence():
    graph = NodeGraph()
    nodeData = ConnectionData()
    nodeType = NodeType()
    
    createdNode = graph.newNode(nodeType, "main", nodeData)

    assert(graph.hasNode("main"));

    # Check that we got back the same
    # node as when we created it.
    retrieved = graph.getNode("main")
    assert(retrieved == createdNode)

    # Check that we get the right
    # behavior for non-existing nodes.
    assert(not graph.hasNode("something-else"))
    assert(graph.getNode("something-else") == None)

    
