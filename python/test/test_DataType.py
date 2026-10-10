import pytest

from nodejs.DataType import DataType

def test_DataType_equality():
    dataType1 = DataType("int", "Integer");
    dataType2 = DataType("int", "Another integer");
    
    assert(dataType1 == dataType2)
    assert(not (dataType1 != dataType2))

    # Things may compare equal as long as the ID is the same.
    # even if they have different names.
    assert(dataType1.getName() != dataType2.getName())
    assert(dataType1.getId() == dataType2.getId())

def test_DataType_copy():
    dataType1 = DataType("int", "Integer");
    dataType2 = DataType("x", "y")
    dataType2.copy(dataType1)
    
    assert(dataType1 == dataType2)

    # Things may compare equal as long as the ID is the same.
    # even if they have different names.
    assert(dataType1.getName() == dataType2.getName())

#    This test case isn't really relevant in Python.
#def test_DataType_assign():
#    test_DataType_copy()
