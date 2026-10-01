import pytest

from nodejs.NodeType import NodeType
from nodejs.NodePort import NodePort

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
    nodeType.addInputPort("first", NodePort("variable", "First Argument"))
    nodeType.addInputPort("second", NodePort("variable", "Second Argument"))
    nodeType.addOutputPort("output", NodePort("variable", "Result"))

    assert (nodeType.getInputPortCount() == 2);
    assert (nodeType.getOutputPortCount() == 1);

    assert (nodeType.getInputPortByName("first").getDescription() == "First Argument");
    assert (nodeType.getInputPortName(0) == "first");
    assert (nodeType.getInputPortByIndex(0).getDescription() == "First Argument");
    assert (nodeType.hasInputPort("first"))
    
    assert (nodeType.getInputPortByName("second").getDescription() == "Second Argument");
    assert (nodeType.getInputPortName(1) == "second");
    assert (nodeType.getInputPortByIndex(1).getDescription() == "Second Argument");

    assert (nodeType.getOutputPortByName("output").getDescription() == "Result");
    assert (nodeType.getOutputPortName(0) == "output");
    assert (nodeType.hasOutputPort("output"))
    assert (nodeType.getOutputPortByIndex(0).getDescription() == "Result");

    # Now, some edge cases:
    assert (nodeType.getInputPortByName("non-existent") == None);
    assert (nodeType.getOutputPortByName("non-existent") == None);
    assert (not nodeType.hasInputPort("non-existent"))
    assert (not nodeType.hasOutputPort("non-existent"))
    assert (nodeType.getInputPortName(99) == "");
    assert (nodeType.getOutputPortName(99) == "");
    assert (nodeType.getInputPortByIndex(99) == None);
    assert (nodeType.getOutputPortByIndex(1) == None);

def test_NodeType_unique_input_ports():
    nodeType = NodeType()
    rc1 = nodeType.addInputPort("first", NodePort("variable", "First Argument"))
    rc2 = nodeType.addInputPort("first", NodePort("variable", "Another"))

    assert (rc1 == True);
    assert (rc2 == False);

    assert (nodeType.getInputPortCount() == 1);
    assert (nodeType.getOutputPortCount() == 0);
    assert (nodeType.getInputPortByName("first").getDescription() == "First Argument");

def test_NodeType_unique_output_ports():
    nodeType = NodeType()
    rc1 = nodeType.addOutputPort("first", NodePort("variable", "First Argument"))
    rc2 = nodeType.addOutputPort("first", NodePort("variable", "Another"))

    assert (rc1 == True);
    assert (rc2 == False);

    assert (nodeType.getInputPortCount() == 0);
    assert (nodeType.getOutputPortCount() == 1);
    assert (nodeType.getOutputPortByName("first").getDescription() == "First Argument");


