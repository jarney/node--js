import pytest

from nodejs.xml import *
from nodejs.NodeModule import NodeModule
import sys

def test_serializer():
    print("Project is");
    print(__package__);

    s = Serializer.instance();

    nm = NodeModule();

    fname = "../doc/openscad.xml"
    stream = open(fname, "r");
    s.read(nm, stream)
    s.write(nm, sys.stdout)
