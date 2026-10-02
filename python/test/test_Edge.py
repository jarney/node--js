from nodejs.Edge import Edge

def test_Edge_equality():
    conn0 = Edge("n0", "p0", "n1", "p1")
    # Only one way that they can be the same.
    conn1 = Edge("n0", "p0", "n1", "p1")
    assert(conn0 == conn1)

    # Lots of different ways it can be different.
    conn2 = Edge("n0", "p1", "n1", "p1")
    conn3 = Edge("n1", "p0", "n1", "p1")
    conn4 = Edge("n0", "p0", "n2", "p1")
    conn5 = Edge("n0", "p0", "n1", "p2")

    assert(conn0 != conn2)
    assert(conn0 != conn3)
    assert(conn0 != conn4)
    assert(conn0 != conn5)

def test_Edge_id():
    conn0 = Edge("n0", "p0", "n1", "p1")
    assert(conn0.getId() == "n0-p0|n1-p1")

