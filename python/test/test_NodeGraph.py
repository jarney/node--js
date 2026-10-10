import pytest

from nodejs.xml import ModuleLoaderNodeJSPath
from nodejs.NodeGraph import NodeGraph
from nodejs.NodeType import NodeType
from nodejs.DataType import DataType
from nodejs.ConnectionData import ConnectionData

def test_NodeGraph_create_node():
    loader = ModuleLoaderNodeJSPath()
    module = loader.newModule("anonymous")
    graph = module.addGraph("graph");
    nodeData = ConnectionData()
    nodeType = NodeType()

    created = graph.newNode(nodeType, "main", nodeData)
    assert(created.getId() == "main")
    
    nextNode0 = graph.newNode(nodeType, "main", nodeData)
    assert(nextNode0.getId() == "main-0")

    nextNode1 = graph.newNode(nodeType, "main", nodeData)
    assert(nextNode1.getId() == "main-1")

def test_NodeGraph_registered_node_type():
    loader = ModuleLoaderNodeJSPath()
    module = loader.newModule("anonymous")
    graph = module.addGraph("graph");
    nodeData = ConnectionData()
    nodeType = NodeType()

    createdNode = graph.newNode(nodeType, "main", nodeData)
    assert(createdNode.getType() == nodeType)
    assert(createdNode.getGraph() == graph)
    

def test_NodeGraph_copied_data():
    loader = ModuleLoaderNodeJSPath()
    module = loader.newModule("anonymous")
    graph = module.addGraph("graph");
    nodeData = ConnectionData()
    nodeType = NodeType()

    nodeData.setValue("initial-value", "some-data")
    createdNode = graph.newNode(nodeType, "main", nodeData)

    # Check that our node's data was initialized with the data
    # we gave it.
    assert(createdNode.getData().hasValue("initial-value"))

    # Check that our node's data is a copy and doesn't
    # refer to the same data.
    createdNode.getData().setValue("runtime-value", "another one")
    assert(createdNode.getData().hasValue("runtime-value"))
    assert(not nodeData.hasValue("runtime-value"))

    nodeData.setValue("runtime-for-initializer", "initializer")
    assert(not createdNode.getData().hasValue("runtime-for-initializer"))
    assert(nodeData.hasValue("runtime-for-initializer"))


def test_NodeGraph_node_existence():
    loader = ModuleLoaderNodeJSPath()
    module = loader.newModule("anonymous")
    graph = module.addGraph("graph");
    nodeData = ConnectionData()
    nodeType = NodeType()
    
    createdNode = graph.newNode(nodeType, "main", nodeData)

    assert(graph.hasNode("main"));

    # Check that we got back the same
    # node as when we created it.
    retrieved = graph.getNode("main")
    assert(retrieved == createdNode)

    # Check that we get the right
    # behavior for non-existing nodes.
    assert(not graph.hasNode("something-else"))
    assert(graph.getNode("something-else") == None)

def test_NodeGraph_create_edge():    
    loader = ModuleLoaderNodeJSPath()
    module = loader.newModule("anonymous")
    graph = module.addGraph("graph");
    
    edgeId = graph.newEdge("a", "first", "b", "second");
    assert(edgeId != None)

def test_NodeGraph_edge_duplicate():
    loader = ModuleLoaderNodeJSPath()
    module = loader.newModule("anonymous")
    graph = module.addGraph("graph");
    
    edgeId = graph.newEdge("a", "first", "b", "second");
    edgeId2 = graph.newEdge("a", "first", "b", "second");
    assert(edgeId != None)
    assert(edgeId2 == None)

def test_NodeGraph_topological_sort():
    loader = ModuleLoaderNodeJSPath()
    module = loader.newModule("anonymous")
    graph = module.addGraph("graph");
    plain = NodeType()
    empty = ConnectionData()

    graph.newNode(plain, "C", empty)
    graph.newNode(plain, "B", empty)
    graph.newNode(plain, "A", empty)
    graph.newNode(plain, "E", empty)

    graph.newEdge("A", "x", "B", "x");
    graph.newEdge("B", "x", "C", "x");
    graph.newEdge("A", "x", "E", "x");
    
    nodesSorted = graph.getNodeIdsInTopologicalOrder()

    assert(nodesSorted == ['C', 'B', 'E', 'A'])
    assert(nodesSorted != ['C', 'B', 'A', 'E'])

def test_NodeGraph_topological_sort_cycle_detection():
    loader = ModuleLoaderNodeJSPath()
    module = loader.newModule("anonymous")
    graph = module.addGraph("graph");
    plain = NodeType()
    empty = ConnectionData()

    graph.newNode(plain, "C", empty)
    graph.newNode(plain, "B", empty)
    graph.newNode(plain, "A", empty)

    graph.newEdge("A", "x", "B", "x");
    graph.newEdge("B", "x", "C", "x");
    graph.newEdge("C", "x", "A", "x");
    
    nodesSorted = graph.getNodeIdsInTopologicalOrder()

    assert(nodesSorted == None)

def test_NodeGraph_getModule():
    loader = ModuleLoaderNodeJSPath()
    nodeModule = loader.newModule("anonymous");
    graph = nodeModule.addGraph("graph");

    assert(nodeModule == graph.getModule())


def test_NodeGraph_erase_node():
    loader = ModuleLoaderNodeJSPath()
    nodeModule = loader.newModule("anonymous")
    graph = nodeModule.addGraph("graph")
    nodeType = NodeType()

    graph.newNode(nodeType, "main", ConnectionData())

    assert(graph.hasNode("main"))
    
    graph.removeNode("main")

    assert(not graph.hasNode("main"))

def test_NodeGraph_erase_node_with_edges():
    loader = ModuleLoaderNodeJSPath()
    nodeModule = loader.newModule("anonymous")
    graph = nodeModule.addGraph("graph")
    nodeType = NodeType()

    graph.newNode(nodeType, "a", ConnectionData());
    graph.newNode(nodeType, "b", ConnectionData());

    assert(graph.hasNode("a"));
    assert(graph.hasNode("b"));

    graph.newEdge("a", "x", "b", "x");
    graph.newEdge("b", "x", "a", "x");
    
    assert(len(graph.getEdgesFrom("a")) == 1);
    assert(len(graph.getEdgesTo("a")) == 1);
    assert(len(graph.getEdgesFrom("b")) == 1);
    assert(len(graph.getEdgesTo("b")) == 1);

    graph.removeNode("a");

    assert(not graph.hasNode("a"));

    assert(len(graph.getEdgesFrom("a")) == 0);
    assert(len(graph.getEdgesTo("a")) == 0);

    assert(len(graph.getEdgesFrom("b")) == 0);
    assert(len(graph.getEdgesTo("b")) == 0);

    assert(len(graph.getEdges()) == 0);

def test_NodeGraph_erase_node_with_extra_edges():
    loader = ModuleLoaderNodeJSPath()
    nodeModule = loader.newModule("anonymous")
    graph = nodeModule.addGraph("graph")
    nodeType = NodeType()

    graph.newNode(nodeType, "a", ConnectionData());
    graph.newNode(nodeType, "b", ConnectionData());
    graph.newNode(nodeType, "c", ConnectionData());
    graph.newNode(nodeType, "d", ConnectionData());

    assert(graph.hasNode("a"));
    assert(graph.hasNode("b"));

    graph.newEdge("a", "x", "b", "x");
    graph.newEdge("b", "x", "a", "x");

    graph.newEdge("a", "x", "c", "x");
    graph.newEdge("b", "x", "d", "x");

    graph.newEdge("c", "x", "d", "x");
    graph.newEdge("d", "x", "c", "x");
    
    assert(len(graph.getEdgesFrom("a")) == 2);
    assert(len(graph.getEdgesTo("a")) == 1);
    assert(len(graph.getEdgesFrom("b")) == 2);
    assert(len(graph.getEdgesTo("b")) == 1);

    graph.removeNode("a");

    assert(not graph.hasNode("a"));

    assert(len(graph.getEdgesFrom("a")) == 0);
    assert(len(graph.getEdgesTo("a")) == 0);

    assert(len(graph.getEdgesFrom("b")) == 1);
    assert(len(graph.getEdgesTo("b")) == 0);

    assert(len(graph.getEdges()) == 3);

def test_NodeGraph_erase_single_edge():
    loader = ModuleLoaderNodeJSPath()
    nodeModule = loader.newModule("anonymous")
    graph = nodeModule.addGraph("graph")
    nodeType = NodeType()

    graph.newNode(nodeType, "a", ConnectionData());
    graph.newNode(nodeType, "b", ConnectionData());
    graph.newNode(nodeType, "c", ConnectionData());

    assert(graph.hasNode("a"));
    assert(graph.hasNode("b"));

    graph.newEdge("a", "x", "b", "x");
    graph.newEdge("a", "p", "c", "q");
    graph.newEdge("c", "p", "b", "q");

    assert(len(graph.getEdgesFrom("a")) == 2);
    assert(len(graph.getEdgesTo("b")) == 2);

    graph.removeEdge("a", "x", "b", "x");

    assert(len(graph.getEdgesFrom("a")) == 1);
    assert(len(graph.getEdgesTo("b")) == 1);

def test_NodeGraph_search_scopes():
    loader = ModuleLoaderNodeJSPath()
    nodeModule = loader.newModule("anonymous")
    graph = nodeModule.addGraph("graph")

    typesScope = loader.newModule("where-we-define-the-types");
    nodeType1 = NodeType();
    nodeType1.setId("type1-node");
    typesScope.addNodeType(nodeType1);

    dataType1 = DataType("foo", "bar");
    typesScope.addDataType(dataType1);

    # Before we add the search scope,
    # we cannot resolve this type.
    assert(graph.getNodeType("type1-node") == None);
    assert(graph.getDataType("foo") == None);

    # Add nodeModule to our search scope.
    graph.addScope(typesScope);

    # After we add the scope, we can find it.
    assert(graph.getNodeType("type1-node") != None);
    assert(graph.getDataType("foo") != None);

    graphCopy = nodeModule.addGraph("graph-copy");

    # Before we copy the scope, we can't resolve it.
    assert(graphCopy.getNodeType("type1-node") == None);
    assert(graphCopy.getDataType("foo") == None);

    graphCopy.copyScope(graph);

    # After copying the scopes, we can resolve the names.
    assert(graphCopy.getNodeType("type1-node") != None);
    assert(graphCopy.getDataType("foo") != None);

    

def test_NodeGraph_groups():
    loader = ModuleLoaderNodeJSPath()
    nodeModule = loader.newModule("anonymous");
    graph = nodeModule.addGraph("graph");

    # Check that basic add and remove works.
    
    assert(not graph.hasGroup("node-group"));
    assert(graph.getGroup("node-group") == None);
    assert(len(graph.getGroups()) == 0);
    
    graph.addGroup("node-group");
    
    assert(graph.hasGroup("node-group"));
    assert(graph.getGroup("node-group") != None);
    assert(len(graph.getGroups()) == 1);

    graph.removeGroup("node-group");

    assert(not graph.hasGroup("node-group"));
    assert(graph.getGroup("node-group") == None);
    assert(len(graph.getGroups()) == 0);

def test_NodeGraph_specific_metadata():
    loader = ModuleLoaderNodeJSPath()
    module = loader.newModule("anonymous");
    graph = module.addGraph("graph");

    metadata = graph.getMetadata();

    # Check that we don't have any specific metadata yet.
    assert(not metadata.hasMetadata("foo.bar.org"));

    # Check that when we ask for metadata, it's created
    cd = metadata.getMetadata("foo.bar.org");
    assert(metadata.hasMetadata("foo.bar.org"));

    cd.setValue("x", "some-value");

    # Python has no such thing as const-ness, so
    # we don't worry about the rest of this test
#    # Check that we can make a const-version of this.
#    const NodeGraph  * constGraph = graph;
#    const Metadata & constMetadata = constGraph.getMetadata();
#    const ConnectionData & constData = constMetadata.getMetadata("foo.bar.org");
#    assert(constData.hasValue("x"));
#
#    const ConnectionData & readonly = constMetadata.getMetadata("some-other-namespace");
#    # Check that in the const context, we didn't actually add the namespace,
#    # just faked it by returning an empty connection data.
#    assert(not constMetadata.hasMetadata("some-other-namespace"));
#    assert(not readonly.hasValue("x"));
