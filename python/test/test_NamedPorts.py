import pytest

from nodejs.NamedPorts import NamedPorts
from nodejs.NodePort import NodePort

DATA_TYPE = "variable"

def test_NamedPorts_features():
    namedPort = NamedPorts()

    namedPort.addPort("a", NodePort(DATA_TYPE, "adesc", NodePort.ConnectionPolicy.Multiple))
    namedPort.addPort("b", NodePort(DATA_TYPE, "bdesc", NodePort.ConnectionPolicy.Multiple))
    namedPort.addPort("c", NodePort(DATA_TYPE, "cdesc", NodePort.ConnectionPolicy.Multiple))
    namedPort.addPort("d", NodePort(DATA_TYPE, "ddesc", NodePort.ConnectionPolicy.Multiple))

    assert(namedPort.hasPort("a"))
    assert(namedPort.hasPort("b"))
    assert(namedPort.hasPort("c"))
    assert(namedPort.hasPort("d"))

    assert(not namedPort.hasPort("z"))

    na = namedPort.getByName("a")
    assert(na != None)
    assert(namedPort.getByName("a").getDescription() == "adesc")
    assert(namedPort.getByName("b").getDescription() == "bdesc")
    assert(namedPort.getByName("c").getDescription() == "cdesc")
    assert(namedPort.getByName("d").getDescription() == "ddesc")

    nnon = namedPort.getByName("z")
    assert(nnon == None)

    assert(namedPort.getPortIndex("a") == 0)
    assert(namedPort.getPortIndex("b") == 1)
    assert(namedPort.getPortIndex("c") == 2)
    assert(namedPort.getPortIndex("d") == 3)
    assert(namedPort.getPortIndex("z") == -1)

    assert(namedPort.getCount() == 4)
    assert(namedPort.getByIndex(0).getDescription() == "adesc")
    assert(namedPort.getByIndex(1).getDescription() == "bdesc")
    assert(namedPort.getByIndex(2).getDescription() == "cdesc")
    assert(namedPort.getByIndex(3).getDescription() == "ddesc")
    assert(namedPort.getByIndex(4) == None)

    assert(namedPort.getName(0) == "a")
    assert(namedPort.getName(1) == "b")
    assert(namedPort.getName(2) == "c")
    assert(namedPort.getName(3) == "d")
