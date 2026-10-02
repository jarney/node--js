from nodejs.Connection import Connection

def test_Connection_equality():
    conn0 = Connection("n0", "p0", "n1", "p1")
    # Only one way that they can be the same.
    conn1 = Connection("n0", "p0", "n1", "p1")
    assert(conn0 == conn1)

    # Lots of different ways it can be different.
    conn2 = Connection("n0", "p1", "n1", "p1")
    conn3 = Connection("n1", "p0", "n1", "p1")
    conn4 = Connection("n0", "p0", "n2", "p1")
    conn5 = Connection("n0", "p0", "n1", "p2")

    assert(conn0 != conn2)
    assert(conn0 != conn3)
    assert(conn0 != conn4)
    assert(conn0 != conn5)

def test_Connection_id():
    conn0 = Connection("n0", "p0", "n1", "p1")
    assert(conn0.getId() == "n0-p0|n1-p1")

