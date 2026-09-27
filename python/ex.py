import xml.etree.ElementTree as ET

from DataType import DataType
from NodeModule import NodeModule

dt = DataType("a", "b")
dt.print()

nm = NodeModule()

nm.addDataType(dt)

fname = "../doc/openscad.xml"
tree = ET.parse(fname)
root = tree.getroot()
print(root)
