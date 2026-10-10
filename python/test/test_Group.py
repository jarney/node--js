import pytest

from nodejs.Group import Group

def test_Group_check_basic_stuff():
    g = Group()

    # Add node, make sure we can find it.
    g.addNode("foo")
    assert(g.contains("foo"))
    assert(g.getNodes() == set(["foo"]))

    # Remove node, make sure it goes away.
    g.removeNode("foo")
    assert(not g.contains("foo"))

    # Make sure non-existing node isnt there.
    assert(not g.contains("bar"))
    g.removeNode("bar")
    assert(not g.contains("bar"))

    assert(len(g.getNodes()) == 0)

