from ..xml import *
from ..NodeModule import NodeModule
import sys

print("Project is");
print(__package__);

s = Serializer.instance();


nm = NodeModule();

fname = "../doc/openscad.xml"
stream = open(fname, "r");
s.read(nm, stream)

#os = open("foo", "w")
s.write(nm, sys.stdout)
