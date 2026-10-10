import pytest

from nodejs.xml import *
from nodejs.NodeModule import NodeModule
from nodejs.SerializerError import SerializerErrorReporterStream
import sys

@pytest.fixture
def data_directory(request: pytest.FixtureRequest) -> Path:
    if request.config.getoption("--data-directory"):
        return request.config.getoption("--data-directory")
    return "."

def getDataFile(data_directory, fname):
    fname = data_directory + "/" + fname
    return fname
    
def test_xml_Serializer(data_directory):
    print("Project is");
    print(__package__);

    err = SerializerErrorReporterStream(sys.stderr)
    s = Serializer.instance();

    loader = ModuleLoaderNodeJSPath()
    nm = loader.newModule("anonymous")

    print(str(type(data_directory)))
    print(data_directory)
    
    fname = getDataFile(data_directory, "Test-Serializer-Basic.xml")
    print(fname)
    with open(fname, "r") as stream:
        s.read(nm, stream, err)
    s.write(nm, sys.stdout, err)

