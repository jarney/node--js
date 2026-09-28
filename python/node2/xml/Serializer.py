from xml.etree import ElementTree as ET

from ..DataType import DataType
from ..NodeModule import NodeModule
#
#dt = DataType("a", "b")
#dt.print()
#
#nm = NodeModule()
#
#nm.addDataType(dt)


class Serializer:
    def __init__(self):
        pass

    def instance():
        return _instance;
    
    def read(self, node_module: NodeModule, stream):
        #fname = "../doc/openscad.xml"
        #stream = open(fname, "r");
        tree = ET.parse(stream)
        root = tree.getroot()
        print(root)

        pass


    def write(self, node_module: NodeModule, stream):
        stream.write("Writing it back out to the stdout port\n");
        pass
    
_instance = Serializer()
