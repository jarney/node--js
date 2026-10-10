import pytest

from nodejs.NodeModule import NodeModule
from nodejs.DataType import DataType
from nodejs.NodeType import NodeType
from nodejs.NodePort import NodePort
from nodejs.xml import ModuleLoaderNodeJSPath

def test_NodeModule_package_id():
    loader = ModuleLoaderNodeJSPath()
    nodeModule = loader.newModule("anonymous")

    nodeModule.setPackage("simple.package.name")
    nodeModule.setDescription("Some Desc")

    assert(nodeModule.getPackage() == "simple.package.name")
    assert(nodeModule.getDescription() == "Some Desc");

def test_NodeModule_single_type():
    loader = ModuleLoaderNodeJSPath()
    nodeModule = loader.newModule("anonymous")
    
    nodeType = NodeType()
    nodeType.setId("add")
    nodeType.getInputs().addPort("first", NodePort("variable", "First Argument"))
    
    nodeModule.addNodeType(nodeType)

    assert(nodeModule.hasNodeType("add"))
    
    nt = nodeModule.getNodeType("add")
    
    assert(nt.getId() == "add")
    assert(nt.getInputs().getCount() == 1)

    nonexistent = nodeModule.getNodeType("invalid-node")
    assert(nonexistent == None)

    # This is actually really dangerous and maybe we should
    # not even have such a method because it would
    # orphan actual nodes in graphs.
    nodeModule.removeNodeType("add")
    assert(not nodeModule.hasNodeType("add"))


def test_NodeModule_iterator():
    loader = ModuleLoaderNodeJSPath()
    nodeModule = loader.newModule("anonymous")
    
    nodeType = NodeType()
    nodeType.setId("add")
    nodeType.getInputs().addPort("first", NodePort("variable", "First Argument"))
    nodeModule.addNodeType(nodeType)

    i = 0
    for it in nodeModule.getNodeTypes():
        assert(it == "add")
        i = i + 1
    # Check that we actually iterated.
    assert(i == 1)

def test_NodeModule_check_basic_data_type_stuff():
    loader = ModuleLoaderNodeJSPath()
    nodeModule = loader.newModule("anonymous")

    dataType = DataType("variable", "Variable Type")
    nodeModule.addDataType(dataType)

    assert(nodeModule.hasDataType("variable"))
    assert(nodeModule.getDataType("variable").getName() == "Variable Type")

    nodeModule.removeDataType("variable")

    assert(not nodeModule.hasDataType("variable"))
    assert(nodeModule.getDataType("variable") == None)
    

def test_NodeModule_data_type_iterator():
    loader = ModuleLoaderNodeJSPath()
    nodeModule = loader.newModule("anonymous")
    
    dataType = DataType("variable", "Variable Type")
    nodeModule.addDataType(dataType)

    i = 0
    for it in nodeModule.getDataTypes():
        assert(it == "variable")
        assert(nodeModule.getDataType(it).getId() == "variable")
        i = i + 1
    # Check that we actually iterated.
    assert(i == 1)

def test_NodeModule_getModuleLoader():
    loader = ModuleLoaderNodeJSPath()
    nodeModule = loader.newModule("anonymous")

    assert(loader == nodeModule.getModuleLoader())

def test_NodeModule_add_graph_uniqueness_conditions():
    loader = ModuleLoaderNodeJSPath()
    nodeModule = loader.newModule("anonymous")

    graph1 = nodeModule.addGraph("graph")
    graph1_out = nodeModule.getGraph("graph")
    assert(graph1 == graph1_out)

    graph2 = nodeModule.addGraph("graph")

    assert(graph1 != None)
    assert(graph2 == None)

    assert(nodeModule.getGraph("nonexistent") == None)

