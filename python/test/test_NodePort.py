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

