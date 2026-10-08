import pytest

from nodejs.NodeType import NodeType
from nodejs.NodePort import NodePort
from nodejs.ConnectionData import ConnectionData

def test_NodeType_empty():
    nodeType = NodeType()
    nodeType.setId("add")
    nodeType.setVisibility(NodeType.Visibility.PRIVATE)
    nodeType.setType(NodeType.Type.NATIVE)

    assert (nodeType.getVisibility() == NodeType.Visibility.PRIVATE);
    assert (nodeType.getType() == NodeType.Type.NATIVE);

    nodeType.setVisibility(NodeType.Visibility.PUBLIC);
    nodeType.setType(NodeType.Type.GRAPH);
    
    assert (nodeType.getVisibility() == NodeType.Visibility.PUBLIC);
    assert (nodeType.getType() == NodeType.Type.GRAPH);

def test_NodeType_some_ports():
    nodeType = NodeType()
    nodeType.setId("add")
    nodeType.getInputs().addPort("first", NodePort("variable", "First Argument"))
    nodeType.getInputs().addPort("second", NodePort("variable", "Second Argument"))
    nodeType.getOutputs().addPort("output", NodePort("variable", "Result"))

    assert (nodeType.getInputs().getCount() == 2);
    assert (nodeType.getOutputs().getCount() == 1);

    assert (nodeType.getInputs().getByName("first").getDescription() == "First Argument");
    assert (nodeType.getInputs().getName(0) == "first");
    assert (nodeType.getInputs().getByIndex(0).getDescription() == "First Argument");
    assert (nodeType.getInputs().hasPort("first"))
    
    assert (nodeType.getInputs().getByName("second").getDescription() == "Second Argument");
    assert (nodeType.getInputs().getName(1) == "second");
    assert (nodeType.getInputs().getByIndex(1).getDescription() == "Second Argument");

    assert (nodeType.getOutputs().getByName("output").getDescription() == "Result");
    assert (nodeType.getOutputs().getName(0) == "output");
    assert (nodeType.getOutputs().hasPort("output"))
    assert (nodeType.getOutputs().getByIndex(0).getDescription() == "Result");

    # Now, some edge cases:
    assert (nodeType.getInputs().getByName("non-existent") == None);
    assert (nodeType.getOutputs().getByName("non-existent") == None);
    assert (not nodeType.getInputs().hasPort("non-existent"))
    assert (not nodeType.getOutputs().hasPort("non-existent"))
    assert (nodeType.getInputs().getName(99) == "");
    assert (nodeType.getOutputs().getName(99) == "");
    assert (nodeType.getInputs().getByIndex(99) == None);
    assert (nodeType.getOutputs().getByIndex(1) == None);

def test_NodeType_unique_input_ports():
    nodeType = NodeType()
    rc1 = nodeType.getInputs().addPort("first", NodePort("variable", "First Argument"))
    rc2 = nodeType.getInputs().addPort("first", NodePort("variable", "Another"))

    assert (rc1 == True);
    assert (rc2 == False);

    assert (nodeType.getInputs().getCount() == 1);
    assert (nodeType.getOutputs().getCount() == 0);
    assert (nodeType.getInputs().getByName("first").getDescription() == "First Argument");

def test_NodeType_unique_output_ports():
    nodeType = NodeType()
    rc1 = nodeType.getOutputs().addPort("first", NodePort("variable", "First Argument"))
    rc2 = nodeType.getOutputs().addPort("first", NodePort("variable", "Another"))

    assert (rc1 == True);
    assert (rc2 == False);

    assert (nodeType.getInputs().getCount() == 0);
    assert (nodeType.getOutputs().getCount() == 1);
    assert (nodeType.getOutputs().getByName("first").getDescription() == "First Argument");

def test_NodeType_default_node_data():
    nodeType = NodeType()

    defaultData = ConnectionData()
    defaultData.setValue("x", "foo")
    nodeType.setDefaultNodeData(defaultData)

    assert(nodeType.getDefaultNodeData().getValue("x") == "foo");
    
