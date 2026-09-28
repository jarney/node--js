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
        self._NAMESPACE="http://jarney.github.io/nodejs-schema"
        pass

    def instance():
        return _instance;

    def readPackage(self, node_module: NodeModule, packageNode):
        if "id" in packageNode.keys():
            node_module.setPackage(packageNode.get("id"))

    def writePackage(self, node_module: NodeModule, root):
        packageNode = ET.Element("package")
        packageNode.set("id", node_module.getPackage())
        root.append(packageNode)
            
    def read(self, node_module: NodeModule, stream):
        tree = ET.parse(stream)
        root = tree.getroot()

        for package in root:
            if package.tag == "{"+ self._NAMESPACE + "}package":
                self.readPackage(node_module, package)
        
        print(root)

        pass


    def write(self, node_module: NodeModule, stream):
        root = ET.Element('node-module')
        root.set("xmlns", "http://jarney.github.io/nodejs-schema")

        self.writePackage(node_module, root)

        dataType = ET.Element("data-types")
        root.append(dataType)

        tree = ET.ElementTree(root)
        ET.indent(tree, space="    ", level=0)
        tree.write(stream, xml_declaration=True, encoding="unicode");
        stream.write("\n")
        pass
    
_instance = Serializer()
