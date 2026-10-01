import pytest

from nodejs.DataType import DataType

def test_DataType_equality():
    dataType1 = DataType("int", "Integer");
    dataType2 = DataType("int", "Another integer");
    

    assert dataType1 == dataType2

    # Things may compare equal as long as the ID is the same.
    # even if they have different names.
    assert dataType1.getName() != dataType2.getName()
