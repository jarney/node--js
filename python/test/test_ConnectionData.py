from nodejs.ConnectionData import ConnectionData

def test_ConnectionData_constructor():
    cd = ConnectionData()

def test_ConnectionData_default_values():
    cd = ConnectionData()

    assert(not cd.hasValue("unknown-value"))
    
    # If we specify a default, check that it's honored.
    assert(cd.getValue("unknown-value", "some-default") == "some-default")

    # If we don't specify a default, it's the empty string.
    assert(cd.getValue("unknown-value") == "")

def test_ConnectionData_actual_values():
    cd = ConnectionData()

    # If we actually have a value, make sure it works.
    cd.setValue("well-known-value", "actual value")
    
    assert(cd.hasValue("well-known-value"))
    assert(cd.getValue("well-known-value", "other-default") == "actual value")

def test_ConnectionData_copy():
    cd = ConnectionData()
    # If we actually have a value, make sure it works.
    cd.setValue("well-known-value", "actual value")

    # Check that whatever we do to one,
    # is done to the other.
    cdOther = cd.copy()
    cdOther.setValue("only-other-value", "blue")

    assert(cdOther.hasValue("well-known-value"))
    assert(cdOther.getValue("well-known-value", "other-default") == "actual value")

    # Check that the new value ONLY shows up on the new list.
    assert(not cd.hasValue("only-other-value"))
    assert(cdOther.hasValue("only-other-value"))

def test_ConnectionData_data():
    cd = ConnectionData()
    # If we actually have a value, make sure it works.
    cd.setValue("well-known-value", "actual value")
    
    d = cd.getData()

    assert("well-known-value" in d)
    assert(d["well-known-value"] == "actual value")
