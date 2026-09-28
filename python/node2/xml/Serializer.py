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

    def readDataType(self, node_module, dataTypeNode):
        dataType = DataType(
            dataTypeNode.get("id", ""),
            dataTypeNode.get("name", "")
        )
        node_module.addDataType(dataType)
        pass
        
    def readDataTypes(self, node_module: NodeModule, dataTypesNode):
        for dataTypeNode in dataTypesNode:
            if not self.isTag(dataTypeNode.tag, "data-type"):
                continue
            self.readDataType(node_module, dataTypeNode)

    def writeDataTypes(self, node_module: NodeModule, root):
        dataTypesNode = ET.Element("data-types")
        root.append(dataTypesNode)
        
        for dataTypeId in node_module.getDataTypes():
            dataType = node_module.getDataType(dataTypeId)
            dataTypeNode = ET.Element("data-type")
            dataTypeNode.set("id", dataType.getId())
            dataTypeNode.set("name", dataType.getName())
            dataTypesNode.append(dataTypeNode)

    def isTag(self, tag, matchTag):
        return tag == "{"+ self._NAMESPACE + "}" + matchTag
 
    def read(self, node_module: NodeModule, stream):
        tree = ET.parse(stream)
        root = tree.getroot()

        for package in root:
            if self.isTag(package.tag, "package"):
                self.readPackage(node_module, package)
        
        for dataTypesNode in root:
            if not self.isTag(dataTypesNode.tag, "data-types"):
                continue
            self.readDataTypes(node_module, dataTypesNode)

        pass


    def write(self, node_module: NodeModule, stream):
        root = ET.Element('node-module')
        root.set("xmlns", "http://jarney.github.io/nodejs-schema")

        self.writePackage(node_module, root)
        self.writeDataTypes(node_module, root)

        tree = ET.ElementTree(root)
        ET.indent(tree, space="    ", level=0)
        tree.write(stream, xml_declaration=True, encoding="unicode");
        stream.write("\n")
        pass
    
_instance = Serializer()
