import { describe, it, expect } from 'vitest'

import { DataType } from "../src/DataType.ts";
import { NodeType } from "../src/NodeType.ts";
import { NodePort } from "../src/NodePort.ts";
import { NodeModule } from "../src/NodeModule.ts"
import { NodeGraph } from "../src/NodeGraph.ts";
import { ConnectionData } from "../src/ConnectionData.ts";
import type { ModuleLoader } from "../src/ModuleLoader.ts"
import { ModuleLoaderNodeJSPath } from "../src/xml/ModuleLoaderNodeJSPath.ts";

describe("NodeGraph", () => {
  it("test_NodeGraph_create_node", function() {
    var loader: ModuleLoader = new ModuleLoaderNodeJSPath();
    var nodeModule: NodeModule = loader.newModule("anonymous");
    var graph: NodeGraph = nodeModule.addGraph("graph");
    var nodeData: ConnectionData = new ConnectionData();
    var nodeType: NodeType = new NodeType();

    // Check that our id generation scheme
    // works to prevent clashes by incrementing
    // a number if we accidentally clash.  This is a 'freecad-like'
    // scheme where it automatically numbers elements if we clash.
    var createdNode: Node = graph.newNode(nodeType, "main", nodeData);
    expect.assert(createdNode.getId() == "main");

    var nextNode0: Node = graph.newNode(nodeType, "main", nodeData);
    expect.assert(nextNode0.getId() == "main-0");
    
    var nextNode1: Node = graph.newNode(nodeType, "main", nodeData);
    expect.assert(nextNode1.getId() == "main-1");
  });

  it("test_NodeGraph_registered_node_type", function() {
    var loader: ModuleLoader = new ModuleLoaderNodeJSPath();
    var nodeModule: NodeModule = loader.newModule("anonymous");
    var graph: NodeGraph = nodeModule.addGraph("graph");;
    var nodeData: ConnectionData = new ConnectionData();
    var nodeType: NodeType = new NodeType();
    
    // Check that the node type always refers to the
    // same node type we gave it to begin with.  Nodes don't
    // carry their own copies of the node data, but instead
    // just refer to the node type you gave it, making
    // the caller responsible for the node type's lifetime.
    
    var createdNode: Node = graph.newNode(nodeType, "main", nodeData);
    
    expect.assert(createdNode.getType() == nodeType);
    expect.assert(createdNode.getGraph() == graph);
  });
  /*

  it("test_NodeGraph_copied_data", function() {
    var loader: ModuleLoader = new ModuleLoaderNodeJSPath();
    var nodeModule: NodeModule = loader.newModule("anonymous");
    var graph: NodeGraph = nodeModule.addGraph("graph");;
    var nodeData: ConnectionData = new ConnectionData();
    var nodeType: NodeType = new NodeType();
    
    nodeData.setValue("initial-value", "some-data");
    var createdNode: Node = graph.newNode(nodeType, "main", nodeData);
    
    // Check that our node's data was initialized with the data
    // we gave it.
    expect.assert(createdNode.getData().hasValue("initial-value"));
    
    // Check that our node's data is a copy and doesn't
    // refer to the same data.
    createdNode.getData().setValue("runtime-value", "another one");
    expect.assert(createdNode.getData().hasValue("runtime-value"));
    expect.assert(!nodeData.hasValue("runtime-value"));
    
    nodeData.setValue("runtime-for-initializer", "initializer");
    expect.assert(!createdNode.getData().hasValue("runtime-for-initializer"));
    expect.assert(nodeData.hasValue("runtime-for-initializer"));
  });

  it("test_NodeGraph_node_existence", function() {
    var loader: ModuleLoader = new ModuleLoaderNodeJSPath();
    var nodeModule: NodeModule = loader.newModule("anonymous");
    var graph: NodeGraph = nodeModule.addGraph("graph");;
    var nodeData: ConnectionData = new ConnectionData();
    var nodeType: NodeType = new NodeType();
    
    var createdNode: Node = graph.newNode(nodeType, "main", nodeData);
    
    expect.assert(graph.hasNode("main"));
    
    // Check that we got back the same
    // node as when we created it.
    var retrieved: Node = graph.getNode("main");
    expect.assert(retrieved == &createdNode);
    
    // Check that we get the right
    // behavior for non-existing nodes.
    expect.assert(!graph.hasNode("something-else"));
    expect.assert(graph.getNode("something-else") == nullptr);
  });

  it("test_NodeGraph_create_edge", function() {
    var loader: ModuleLoader = new ModuleLoaderNodeJSPath();
    var nodeModule: NodeModule = loader.newModule("anonymous");
    var graph: NodeGraph = nodeModule.addGraph("graph");;
    std::optional<EdgeId> edgeId = graph.newEdge("a", "first", "b", "second");
    expect.assert(edgeId.has_value());
  });

  it("test_NodeGraph_edge_duplicate", function() {
    var loader: ModuleLoader = new ModuleLoaderNodeJSPath();
    var nodeModule: NodeModule = loader.newModule("anonymous");
    var graph: NodeGraph = nodeModule.addGraph("graph");;
    std::optional<EdgeId> edgeId = graph.newEdge("a", "first", "b", "second");
    std::optional<EdgeId> edgeId2 = graph.newEdge("a", "first", "b", "second");
    expect.assert(edgeId.has_value());
    expect.assert(!edgeId2.has_value());
  });

  it("test_NodeGraph_topological_sort", function() {
    var loader: ModuleLoader = new ModuleLoaderNodeJSPath();
    var nodeModule: NodeModule = loader.newModule("anonymous");
    var graph: NodeGraph = nodeModule.addGraph("main");
    NodeType plain;

    graph.newNode(plain, "C");
    graph.newNode(plain, "B");
    graph.newNode(plain, "A");
    graph.newNode(plain, "E");

    graph.newEdge("A", "x", "B", "x");
    graph.newEdge("B", "x", "C", "x");
    graph.newEdge("A", "x", "E", "x");
    graph.newEdge("E", "x", "B", "x");
    
    std::optional<std::vector<NodeId>> maybeNodesSorted = graph.getNodeIdsInTopologicalOrder();
    expect.assert(maybeNodesSorted.has_value());

    std::vector<NodeId> nodesSorted = maybeNodesSorted.value();
    std::vector<NodeId> correctOrder = {"A", "E", "B", "C"};
    std::vector<NodeId> incorrectOrder = {"C", "B", "A", "E"};
    expect.assert(nodesSorted == correctOrder);
    expect.assert(nodesSorted != incorrectOrder);
});

it("test_NodeGraph_topological_sort_cycle_detection", function() {
    var loader: ModuleLoader = new ModuleLoaderNodeJSPath();
    var nodeModule: NodeModule = loader.newModule("anonymous");
  var graph: NodeGraph = nodeModule.addGraph("main");
  var plain: NodeType = new NodeType();
    
    graph.newNode(plain, "C");
    graph.newNode(plain, "B");
    graph.newNode(plain, "A");
    
    graph.newEdge("A", "x", "B", "x");
    graph.newEdge("B", "x", "C", "x");
    graph.newEdge("C", "x", "A", "x");
    
    std::optional<std::vector<NodeId>> maybeNodesSorted = graph.getNodeIdsInTopologicalOrder();
    expect.assert(!maybeNodesSorted.has_value());    
});

it("test_NodeGraph_getModule", function() {
    var loader: ModuleLoader = new ModuleLoaderNodeJSPath();
    var nodeModule: NodeModule = loader.newModule("anonymous");
    NodeGraph & graph = *nodeModule.addGraph("graph");

    expect.assert(&nodeModule == &graph.getModule());

});

it("test_NodeGraph_erase_node", function() {
    var loader: ModuleLoader = new ModuleLoaderNodeJSPath();
    var nodeModule: NodeModule = loader.newModule("anonymous");
    NodeGraph & graph = *nodeModule.addGraph("graph");
    var nodeType: NodeType = new NodeType();

    graph.newNode(nodeType, "main");

    expect.assert(graph.hasNode("main"));
    
    graph.removeNode("main");

    expect.assert(!graph.hasNode("main"));
});

it("test_NodeGraph_erase_node_with_edges", function() {
    var loader: ModuleLoader = new ModuleLoaderNodeJSPath();
    var nodeModule: NodeModule = loader.newModule("anonymous");
    NodeGraph & graph = *nodeModule.addGraph("graph");
    var nodeType: NodeType = new NodeType();

    graph.newNode(nodeType, "a");
    graph.newNode(nodeType, "b");

    expect.assert(graph.hasNode("a"));
    expect.assert(graph.hasNode("b"));

    graph.newEdge("a", "x", "b", "x");
    graph.newEdge("b", "x", "a", "x");
    
    expect.assert(graph.getEdgesFrom("a").size() == 1);
    expect.assert(graph.getEdgesTo("a").size() == 1);
    expect.assert(graph.getEdgesFrom("b").size() == 1);
    expect.assert(graph.getEdgesTo("b").size() == 1);

    graph.removeNode("a");

    expect.assert(!graph.hasNode("a"));

    expect.assert(graph.getEdgesFrom("a").size() == 0);
    expect.assert(graph.getEdgesTo("a").size() == 0);

    expect.assert(graph.getEdgesFrom("b").size() == 0);
    expect.assert(graph.getEdgesTo("b").size() == 0);

    expect.assert(graph.getEdges().size() == 0);
});

it("test_NodeGraph_erase_node_with_extra_edges", function() {
    var loader: ModuleLoader = new ModuleLoaderNodeJSPath();
    var nodeModule: NodeModule = loader.newModule("anonymous");
    NodeGraph & graph = *nodeModule.addGraph("graph");
    var nodeType: NodeType = new NodeType();

    graph.newNode(nodeType, "a");
    graph.newNode(nodeType, "b");
    graph.newNode(nodeType, "c");
    graph.newNode(nodeType, "d");

    expect.assert(graph.hasNode("a"));
    expect.assert(graph.hasNode("b"));

    graph.newEdge("a", "x", "b", "x");
    graph.newEdge("b", "x", "a", "x");

    graph.newEdge("a", "x", "c", "x");
    graph.newEdge("b", "x", "d", "x");

    graph.newEdge("c", "x", "d", "x");
    graph.newEdge("d", "x", "c", "x");
    
    expect.assert(graph.getEdgesFrom("a").size() == 2);
    expect.assert(graph.getEdgesTo("a").size() == 1);
    expect.assert(graph.getEdgesFrom("b").size() == 2);
    expect.assert(graph.getEdgesTo("b").size() == 1);

    graph.removeNode("a");

    expect.assert(!graph.hasNode("a"));

    expect.assert(graph.getEdgesFrom("a").size() == 0);
    expect.assert(graph.getEdgesTo("a").size() == 0);

    expect.assert(graph.getEdgesFrom("b").size() == 1);
    expect.assert(graph.getEdgesTo("b").size() == 0);

    expect.assert(graph.getEdges().size() == 3);
});

it("test_NodeGraph_erase_single_edge", function() {
    var loader: ModuleLoader = new ModuleLoaderNodeJSPath();
    var nodeModule: NodeModule = loader.newModule("anonymous");
    NodeGraph & graph = *nodeModule.addGraph("graph");
    var nodeType: NodeType = new NodeType();

    graph.newNode(nodeType, "a");
    graph.newNode(nodeType, "b");
    graph.newNode(nodeType, "c");

    expect.assert(graph.hasNode("a"));
    expect.assert(graph.hasNode("b"));

    graph.newEdge("a", "x", "b", "x");
    graph.newEdge("a", "p", "c", "q");
    graph.newEdge("c", "p", "b", "q");

    expect.assert(graph.getEdgesFrom("a").size() == 2);
    expect.assert(graph.getEdgesTo("b").size() == 2);

    graph.removeEdge("a", "x", "b", "x");

    expect.assert(graph.getEdgesFrom("a").size() == 1);
    expect.assert(graph.getEdgesTo("b").size() == 1);
});

it("test_NodeGraph_search_scopes", function() {
    var loader: ModuleLoader = new ModuleLoaderNodeJSPath();
    var nodeModule: NodeModule = loader.newModule("anonymous");
    NodeGraph & graph = *nodeModule.addGraph("graph");

    NodeModule & typesScope = *loader.newModule("where-we-define-the-types");
    auto nodeType1 = std::make_unique<NodeType>();
    nodeType1.setId("type1-node");
    typesScope.addNodeType(std::move(nodeType1));

    auto dataType1 = std::make_unique<DataType>("foo", "bar");
    typesScope.addDataType(std::move(dataType1));

    // Before we add the search scope,
    // we cannot resolve this type.
    expect.assert(graph.getNodeType("type1-node") == nullptr);
    expect.assert(graph.getDataType("foo") == nullptr);

    // Add nodeModule to our search scope.
    graph.addScope(&typesScope);

    // After we add the scope, we can find it.
    expect.assert(graph.getNodeType("type1-node") != nullptr);
    expect.assert(graph.getDataType("foo") != nullptr);

    NodeGraph & graphCopy = *nodeModule.addGraph("graph-copy");

    // Before we copy the scope, we can't resolve it.
    expect.assert(graphCopy.getNodeType("type1-node") == nullptr);
    expect.assert(graphCopy.getDataType("foo") == nullptr);

    graphCopy.copyScope(&graph);

    // After copying the scopes, we can resolve the names.
    expect.assert(graphCopy.getNodeType("type1-node") != nullptr);
    expect.assert(graphCopy.getDataType("foo") != nullptr);

    
});

it("test_NodeGraph_groups", function() {
    var loader: ModuleLoader = new ModuleLoaderNodeJSPath();
    var nodeModule: NodeModule = loader.newModule("anonymous");
    NodeGraph & graph = *nodeModule.addGraph("graph");

    // Check that basic add and remove works.
    
    expect.assert(!graph.hasGroup("node-group"));
    expect.assert(graph.getGroup("node-group") == nullptr);
    expect.assert(graph.getGroups().size() == 0);
    
    graph.addGroup("node-group");
    
    expect.assert(graph.hasGroup("node-group"));
    expect.assert(graph.getGroup("node-group") != nullptr);
    expect.assert(graph.getGroups().size() == 1);

    graph.removeGroup("node-group");

    expect.assert(!graph.hasGroup("node-group"));
    expect.assert(graph.getGroup("node-group") == nullptr);
    expect.assert(graph.getGroups().size() == 0);
});

it("test_NodeGraph_specific_metadata", function() {
    var loader: ModuleLoader = new ModuleLoaderNodeJSPath();
    NodeModule & module = *loader.newModule("anonymous");
    var graph: NodeGraph = module.addGraph("graph");

    Metadata & metadata = graph.getMetadata();

    // Check that we don't have any specific metadata yet.
    expect.assert(!metadata.hasMetadata("foo.bar.org"));

    // Check that when we ask for metadata, it's created
    ConnectionData & cd = metadata.getMetadata("foo.bar.org");
    expect.assert(metadata.hasMetadata("foo.bar.org"));

    cd.setValue("x", "some-value");

    // Check that we can make a const-version of this.
    const NodeGraph * constGraph = graph;
    const Metadata & constMetadata = constGraph.getMetadata();
    const ConnectionData & constData = constMetadata.getMetadata("foo.bar.org");
    expect.assert(constData.hasValue("x"));

    const ConnectionData & readonly = constMetadata.getMetadata("some-other-namespace");
    // Check that in the const context, we didn't actually add the namespace,
    // just faked it by returning an empty connection data.
    expect.assert(!constMetadata.hasMetadata("some-other-namespace"));
    expect.assert(!readonly.hasValue("x"));
});
   */
});
