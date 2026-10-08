import pytest

from nodejs.Metadata import Metadata

def test_Metadata_equality():
    metadata = Metadata();

    assert(not metadata.hasMetadata("some-entry"));
    
    # Retrieving a non-existent entry should create it.
    cd = metadata.getMetadata("some-entry")
    cd.setValue("a", "b");
    assert(cd.getValue("a") == "b")


    # Check that when we get it again, it already exists and has our data.
    assert(metadata.hasMetadata("some-entry"));
    cd2 = metadata.getMetadata("some-entry")
    assert(cd2.getValue("a") == "b")

    assert(metadata.getNamespaces() == ["some-entry"])


