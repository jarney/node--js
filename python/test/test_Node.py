import pytest

from nodejs.Node import Node
from nodejs.NodePort import NodePort
from nodejs.NodeType import NodeType
from nodejs.Metadata import Metadata
from nodejs.ConnectionData import ConnectionData
from nodejs.xml import ModuleLoaderNodeJSPath
from nodejs.xml import Serializer

DATA_TYPE = "variable"

def test_Node_create_node():
    loader = ModuleLoaderNodeJSPath()
    module = loader.newModule("anonymous")
    graph = module.addGraph("graph");
    
    nodeType = NodeType()

    # First, just check identity stuff.
    createdNode = graph.newNode(nodeType, "main", ConnectionData())
    assert(createdNode.getId() == "main");
    assert(createdNode.getType() == nodeType);

def test_Node_type_io():
    loader = ModuleLoaderNodeJSPath()
    module = loader.newModule("anonymous")
    graph = module.addGraph("graph");

    nodeType = NodeType()
    nodeType.getInputs().addPort("a", NodePort(DATA_TYPE, "adesc", NodePort.ConnectionPolicy.Multiple));
    nodeType.getInputs().addPort("b", NodePort(DATA_TYPE, "bdesc", NodePort.ConnectionPolicy.Multiple));
    nodeType.getOutputs().addPort("out", NodePort(DATA_TYPE, "out", NodePort.ConnectionPolicy.Multiple));

    node = graph.newNode(nodeType, "main", ConnectionData())
    assert(node.getType().getInputs().getCount() == 2);
    assert(node.getType().getOutputs().getCount() == 1);

    # So we can validate that the node type's data is correct,
    # but we should also verify the node-specific port info.
    assert(node.getType().getInputs().getByName("a").getDescription() == "adesc");
    assert(node.getType().getInputs().getByName("b").getDescription() == "bdesc");
    assert(node.getType().getOutputs().getByName("out").getDescription() == "out");

    assert(node.getInputs().getByName("a").getDescription() == "adesc");
    assert(node.getInputs().getByName("b").getDescription() == "bdesc");
    assert(node.getOutputs().getByName("out").getDescription() == "out");


def test_Node_specific_io():
    loader = ModuleLoaderNodeJSPath()
    module = loader.newModule("anonymous")
    graph = module.addGraph("graph");

    nodeType = NodeType()

    node = graph.newNode(nodeType, "main", ConnectionData())
    
    node.getOverrideInputs().addPort("a", NodePort(DATA_TYPE, "adesc", NodePort.ConnectionPolicy.Multiple));
    node.getOverrideInputs().addPort("b", NodePort(DATA_TYPE, "bdesc", NodePort.ConnectionPolicy.Multiple));
    node.getOverrideOutputs().addPort("out", NodePort(DATA_TYPE, "out", NodePort.ConnectionPolicy.Multiple));

    assert(not node.hasOverrideInputs());
    assert(not node.hasOverrideOutputs());

    # Check that we've set the override ports
    assert(node.getOverrideInputs().getCount() == 2);
    assert(node.getOverrideOutputs().getCount() == 1);

    # Check that they have not yet taken effect.
    assert(node.getInputs().getCount() == 0);
    assert(node.getOutputs().getCount() == 0);

    # When we set the node to override, the port counts
    # should change based on what we set up.
    node.setOverrideInputs(True);
    node.setOverrideOutputs(True);

    assert(node.hasOverrideInputs());
    assert(node.hasOverrideOutputs());
    assert(node.getInputs().getCount() == 2);
    assert(node.getOutputs().getCount() == 1);

def test_Node_initialization_with_data():
    loader = ModuleLoaderNodeJSPath()
    module = loader.newModule("anonymous")
    graph = module.addGraph("graph");

    nodeType = NodeType()
    initialData = ConnectionData()
    initialData.setValue("x", "some-initial-value");
    
    node = graph.newNode(nodeType, "main", initialData);

    # Verify that we initialized the data
    assert(node.getData().getValue("x") == "some-initial-value");

    # Verify that if we change the initial data, the node doesn't change
    initialData.setValue("x", "changed our default");
    
    assert(initialData.getValue("x") == "changed our default");
    assert(node.getData().getValue("x") == "some-initial-value");

    # Verify that if we change the node's data, it
    # is different than the original copy.
    node.getData().setValue("x", "changed node-specific data");
    
    assert(initialData.getValue("x") == "changed our default");
    assert(node.getData().getValue("x") == "changed node-specific data");

def test_Node_specific_metadata():
    loader = ModuleLoaderNodeJSPath()
    module = loader.newModule("anonymous")
    graph = module.addGraph("graph");

    nodeType = NodeType()

    node = graph.newNode(nodeType, "main", ConnectionData());

    metadata = node.getMetadata();

    # Check that we don't have any specific metadata yet.
    assert(not metadata.hasMetadata("foo.bar.org"));

    # Check that when we ask for metadata, it's created
    cd = metadata.getMetadata("foo.bar.org");
    assert(metadata.hasMetadata("foo.bar.org"));

    cd.setValue("x", "some-value");

    # Python has no such thing as const-ness, so
    # we don't worry about the rest of this test
#    # Check that we can make a const-version of this.
#    const Node  constNode = node;
#    const Metadata  constMetadata = constNode.getMetadata();
#    const ConnectionData  constData = constMetadata.getMetadata("foo.bar.org");
#    assert(constData.hasValue("x"));
#
#    const ConnectionData  readonly = constMetadata.getMetadata("some-other-namespace");
#    # Check that in the const context, we didn't actually add the namespace,
#    # just faked it by returning an empty connection data.
#    assert(not constMetadata.hasMetadata("some-other-namespace"));
#    assert(not readonly.hasValue("x"));

