from xml.etree import ElementTree as ET

from ..DataType import DataType
from ..NodeModule import NodeModule
from ..NodeType import NodeType
from ..NodePort import NodePort

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

    def readInputs(self, nodeType: NodeType, inputsNode):
        for portNode in inputsNode:
            if not self.isTag(portNode.tag, "port"):
                continue
            policy = NodePort.ConnectionPolicy.One
            if portNode.get("connection-policy", "") == "multi":
                policy = NodePort.ConnectionPolicy.Multiple
            port = NodePort(portNode.get("data-type"),
                            portNode.get("description"),
                            policy)
            nodeType.addInputPort(portNode.get("id"), port)

    def readOutputs(self, nodeType: NodeType, outputsNode):
        for portNode in outputsNode:
            if not self.isTag(portNode.tag, "port"):
                continue
            policy = NodePort.ConnectionPolicy.One
            if portNode.get("connection-policy", "") == "multi":
                policy = NodePort.ConnectionPolicy.Multiple
            port = NodePort(portNode.get("data-type"),
                            portNode.get("description"),
                            policy)
            nodeType.addOutputPort(portNode.get("id"), port)
            
    def readNodeType(self, node_module: NodeModule, nodeTypeNode):
        nodeType = NodeType()
        nodeType.setId(nodeTypeNode.get("id", ""))

        for portsNode in nodeTypeNode:
            if self.isTag(portsNode.tag, "inputs"):
                self.readInputs(nodeType, portsNode)
            elif self.isTag(portsNode.tag, "outputs"):
                self.readOutputs(nodeType, portsNode)

        node_module.addNodeType(nodeType)

    def readNodeTypes(self, node_module: NodeModule, nodeTypesNode):
        for nodeTypeNode in nodeTypesNode:
            if not self.isTag(nodeTypeNode.tag, "node-type"):
                continue
            self.readNodeType(node_module, nodeTypeNode)

    def writeNodeType(self, nodeType: NodeType, nodeTypesNode):
        nodeTypeNode = ET.Element("node-type")
        nodeTypeNode.set("id", nodeType.getId())
        nodeTypeNode.set("visibility", nodeType.getVisibility())
        nodeTypesNode.append(nodeTypeNode)

        inputsNode = ET.Element("inputs")
        for i in range(0, nodeType.getInputPortCount()):
            portNode = ET.Element("port")
            port = nodeType.getInputPortByIndex(i)
            portNode.set("id", nodeType.getInputPortName(i))
            portNode.set("data-type", port.getDataType())
            connectionPolicy = "multi" if port.getConnectionPolicy() == NodePort.ConnectionPolicy.Multiple else "one"
            portNode.set("connection-policy", connectionPolicy);
            inputsNode.append(portNode)
        nodeTypeNode.append(inputsNode)
        
        outputsNode = ET.Element("outputs")
        for i in range(0, nodeType.getOutputPortCount()):
            portNode = ET.Element("port")
            port = nodeType.getOutputPortByIndex(i)
            portNode.set("id", nodeType.getOutputPortName(i))
            portNode.set("data-type", port.getDataType())
            connectionPolicy = "multi" if port.getConnectionPolicy() == NodePort.ConnectionPolicy.Multiple else "one"
            portNode.set("connection-policy", connectionPolicy);
            outputsNode.append(portNode)
        nodeTypeNode.append(outputsNode)
            
    def writeNodeTypes(self, node_module: NodeModule, root):
        nodeTypesNode = ET.Element("node-types")
        root.append(nodeTypesNode);

        for nodeTypeId in node_module.getNodeTypes():
            nodeType = node_module.getNodeType(nodeTypeId)
            self.writeNodeType(nodeType, nodeTypesNode)
            
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

        for nodeTypesNode in root:
            if not self.isTag(nodeTypesNode.tag, "node-types"):
                continue
            self.readNodeTypes(node_module, nodeTypesNode)

        pass


    def write(self, node_module: NodeModule, stream):
        root = ET.Element('node-module')
        root.set("xmlns", "http://jarney.github.io/nodejs-schema")

        self.writePackage(node_module, root)
        self.writeDataTypes(node_module, root)
        self.writeNodeTypes(node_module, root)

        tree = ET.ElementTree(root)
        ET.indent(tree, space="    ", level=0)
        tree.write(stream, xml_declaration=True, encoding="unicode");
        stream.write("\n")
        pass
    
_instance = Serializer()
