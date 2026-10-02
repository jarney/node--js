import pytest

from nodejs.xml import *
from nodejs.NodeModule import NodeModule
import sys

@pytest.fixture
def data_directory(request: pytest.FixtureRequest) -> Path:
    if request.config.getoption("--data-directory"):
        return request.config.getoption("--data-directory")
    return "."

def getDataFile(data_directory, fname):
    fname = data_directory + "/" + fname
    return fname
    
def test_serializer(data_directory):
    print("Project is");
    print(__package__);

    s = Serializer.instance();

    nm = NodeModule();

    print(str(type(data_directory)))
    print(data_directory)
    
    fname = getDataFile(data_directory, "Test-Serializer-Basic.xml")
    print(fname)
    stream = open(fname, "r");
    s.read(nm, stream)
    s.write(nm, sys.stdout)

