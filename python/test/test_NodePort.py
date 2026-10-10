import pytest

from nodejs.NodePort import NodePort

DATA_TYPE = "variable"
DESCRIPTION = "Input A"

def test_NodePort_multiple():
    nodePort = NodePort(DATA_TYPE, DESCRIPTION, NodePort.ConnectionPolicy.Multiple)

    assert (nodePort.getDataType() == DATA_TYPE)
    assert (nodePort.getDescription() == DESCRIPTION)
    assert (nodePort.getConnectionPolicy() == NodePort.ConnectionPolicy.Multiple)

def test_NodePort_default():
    nodePort = NodePort(DATA_TYPE, DESCRIPTION)

    assert (nodePort.getDataType() == DATA_TYPE)
    assert (nodePort.getDescription() == DESCRIPTION)
    assert (nodePort.getConnectionPolicy() == NodePort.ConnectionPolicy.One)


def test_NodePort_explicit_one():
    nodePort = NodePort(DATA_TYPE, DESCRIPTION, NodePort.ConnectionPolicy.One)

    assert(nodePort.getDataType() == DATA_TYPE)
    assert(nodePort.getDescription() == DESCRIPTION)
    assert(nodePort.getConnectionPolicy() == NodePort.ConnectionPolicy.One)


def test_NodePort_specific_metadata():
    nodePort = NodePort(DATA_TYPE, DESCRIPTION, NodePort.ConnectionPolicy.One)

    metadata = nodePort.getMetadata();

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

