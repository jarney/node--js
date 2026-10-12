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
    expect.assert(retrieved == createdNode);
    
    // Check that we get the right
    // behavior for non-existing nodes.
    expect.assert(!graph.hasNode("something-else"));
    expect.assert(graph.getNode("something-else") == undefined);
  });

  it("test_NodeGraph_create_edge", function() {
    var loader: ModuleLoader = new ModuleLoaderNodeJSPath();
    var nodeModule: NodeModule = loader.newModule("anonymous");
    var graph: NodeGraph = nodeModule.addGraph("graph");;
    var edgeId: EdgeId? = graph.newEdge("a", "first", "b", "second");
    expect.assert(edgeId != undefined);
  });

  it("test_NodeGraph_edge_duplicate", function() {
    var loader: ModuleLoader = new ModuleLoaderNodeJSPath();
    var nodeModule: NodeModule = loader.newModule("anonymous");
    var graph: NodeGraph = nodeModule.addGraph("graph");;
    var edgeId: Edge?  = graph.newEdge("a", "first", "b", "second");
    var edgeId2: Edge? = graph.newEdge("a", "first", "b", "second");
    expect.assert(edgeId != undefined);
    expect.assert(edgeId2 == undefined);
  });

  it("test_NodeGraph_topological_sort", function() {
    var loader: ModuleLoader = new ModuleLoaderNodeJSPath();
    var nodeModule: NodeModule = loader.newModule("anonymous");
    var graph: NodeGraph = nodeModule.addGraph("main");
    var plain: NodeType = new NodeType();

    graph.newNode(plain, "C");
    graph.newNode(plain, "B");
    graph.newNode(plain, "A");
    graph.newNode(plain, "E");

    graph.newEdge("A", "x", "B", "x");
    graph.newEdge("B", "x", "C", "x");
    graph.newEdge("A", "x", "E", "x");
    graph.newEdge("E", "x", "B", "x");
    
    var maybeNodesSorted: NodeId[] | undefined = graph.getNodeIdsInTopologicalOrder();
    expect.assert(maybeNodesSorted != undefined);

    var nodesSorted : NodeId[] = maybeNodesSorted!;
    var correctOrder: NodeId[] = ["A", "E", "B", "C"];
    var correctOrder2: NodeId[] = ["E", "A", "B", "C"];
    var incorrectOrder: NodeId[] = ["C", "B", "A", "E"];
    expect.assert(
      JSON.stringify(nodesSorted) == JSON.stringify(correctOrder) ||
      JSON.stringify(nodesSorted) == JSON.stringify(correctOrder2)
    );
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
    
    var maybeNodesSorted: NodeId[] | undefined = graph.getNodeIdsInTopologicalOrder();
    expect.assert(maybeNodesSorted == undefined);
  });

  it("test_NodeGraph_topological_sort_self_cycle", function() {
    var loader: ModuleLoader = new ModuleLoaderNodeJSPath();
    var nodeModule: NodeModule = loader.newModule("anonymous");
    var graph: NodeGraph = nodeModule.addGraph("main");
    var plain: NodeType = new NodeType();
    
    graph.newNode(plain, "C");
    
    graph.newEdge("C", "x", "C", "x");
    
    var maybeNodesSorted: NodeId[] | undefined = graph.getNodeIdsInTopologicalOrder();
    expect.assert(maybeNodesSorted == undefined);
  });

  it("test_NodeGraph_getModule", function() {
    var loader: ModuleLoader = new ModuleLoaderNodeJSPath();
    var nodeModule: NodeModule = loader.newModule("anonymous");
    var graph: NodeGraph = nodeModule.addGraph("graph");
    
    expect.assert(nodeModule == graph.getModule());
  });
  
  it("test_NodeGraph_erase_node", function() {
    var loader: ModuleLoader = new ModuleLoaderNodeJSPath();
    var nodeModule: NodeModule = loader.newModule("anonymous");
    var graph: NodeGraph = nodeModule.addGraph("graph");
    var nodeType: NodeType = new NodeType();

    graph.newNode(nodeType, "main");

    expect.assert(graph.hasNode("main"));
    
    graph.removeNode("main");

    expect.assert(!graph.hasNode("main"));
  });

  it("test_NodeGraph_erase_node_with_edges", function() {
    var loader: ModuleLoader = new ModuleLoaderNodeJSPath();
    var nodeModule: NodeModule = loader.newModule("anonymous");
    var graph: NodeGraph = nodeModule.addGraph("graph");
    var nodeType: NodeType = new NodeType();
    
    graph.newNode(nodeType, "a");
    graph.newNode(nodeType, "b");

    expect.assert(graph.hasNode("a"));
    expect.assert(graph.hasNode("b"));

    graph.newEdge("a", "x", "b", "x");
    graph.newEdge("b", "x", "a", "x");
    
    expect.assert(graph.getEdgesFrom("a").length == 1);
    expect.assert(graph.getEdgesTo("a").length == 1);
    expect.assert(graph.getEdgesFrom("b").length == 1);
    expect.assert(graph.getEdgesTo("b").length == 1);

    graph.removeNode("a");

    expect.assert(!graph.hasNode("a"));

    expect.assert(graph.getEdgesFrom("a").length == 0);
    expect.assert(graph.getEdgesTo("a").length == 0);

    expect.assert(graph.getEdgesFrom("b").length == 0);
    expect.assert(graph.getEdgesTo("b").length == 0);

    expect.assert(Object.keys(graph.getEdges()).length == 0);
  });
  it("test_NodeGraph_erase_node_with_extra_edges", function() {
    var loader: ModuleLoader = new ModuleLoaderNodeJSPath();
    var nodeModule: NodeModule = loader.newModule("anonymous");
    var graph: NodeGraph = nodeModule.addGraph("graph");
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
    
    expect.assert(graph.getEdgesFrom("a").length == 2);
    expect.assert(graph.getEdgesTo("a").length == 1);
    expect.assert(graph.getEdgesFrom("b").length == 2);
    expect.assert(graph.getEdgesTo("b").length == 1);

    graph.removeNode("a");

    expect.assert(!graph.hasNode("a"));

    expect.assert(graph.getEdgesFrom("a").length == 0);
    expect.assert(graph.getEdgesTo("a").length == 0);

    expect.assert(graph.getEdgesFrom("b").length == 1);
    expect.assert(graph.getEdgesTo("b").length == 0);

    expect.assert(Object.keys(graph.getEdges()).length == 3);
  });
  it("test_NodeGraph_erase_single_edge", function() {
    var loader: ModuleLoader = new ModuleLoaderNodeJSPath();
    var nodeModule: NodeModule = loader.newModule("anonymous");
    var graph: NodeGraph = nodeModule.addGraph("graph");
    var nodeType: NodeType = new NodeType();

    graph.newNode(nodeType, "a");
    graph.newNode(nodeType, "b");
    graph.newNode(nodeType, "c");

    expect.assert(graph.hasNode("a"));
    expect.assert(graph.hasNode("b"));

    graph.newEdge("a", "x", "b", "x");
    graph.newEdge("a", "p", "c", "q");
    graph.newEdge("c", "p", "b", "q");

    expect.assert(graph.getEdgesFrom("a").length == 2);
    expect.assert(graph.getEdgesTo("b").length == 2);

    graph.removeEdge("a", "x", "b", "x");

    expect.assert(graph.getEdgesFrom("a").length == 1);
    expect.assert(graph.getEdgesTo("b").length == 1);
  });

  it("test_NodeGraph_search_scopes", function() {
    var loader: ModuleLoader = new ModuleLoaderNodeJSPath();
    var nodeModule: NodeModule = loader.newModule("anonymous");
    var graph: NodeGraph = nodeModule.addGraph("graph");
    
    var typesScope: NodeModule = loader.newModule("where-we-define-the-types");
    var nodeType1: NodeType = new NodeType();
    nodeType1.setId("type1-node");
    typesScope.addNodeType(nodeType1);

    var dataType1: DataType = new DataType("foo", "bar");
    typesScope.addDataType(dataType1);

    // Before we add the search scope,
    // we cannot resolve this type.
    expect.assert(graph.getNodeType("type1-node") == undefined);
    expect.assert(graph.getDataType("foo") == undefined);

    // Add nodeModule to our search scope.
    graph.addScope(typesScope);

    // After we add the scope, we can find it.
    expect.assert(graph.getNodeType("type1-node") != undefined);
    expect.assert(graph.getDataType("foo") != undefined);

    var graphCopy: NodeGraph = nodeModule.addGraph("graph-copy");

    // Before we copy the scope, we can't resolve it.
    expect.assert(graphCopy.getNodeType("type1-node") == undefined);
    expect.assert(graphCopy.getDataType("foo") == undefined);

    graphCopy.copyScope(graph);

    // After copying the scopes, we can resolve the names.
    expect.assert(graphCopy.getNodeType("type1-node") != undefined);
    expect.assert(graphCopy.getDataType("foo") != undefined);
  });

  it("test_NodeGraph_groups", function() {
    var loader: ModuleLoader = new ModuleLoaderNodeJSPath();
    var nodeModule: NodeModule = loader.newModule("anonymous");
    var graph: NodeGraph = nodeModule.addGraph("graph");

    // Check that basic add and remove works.
    
    expect.assert(!graph.hasGroup("node-group"));
    expect.assert(graph.getGroup("node-group") == undefined);
    expect.assert(Object.keys(graph.getGroups()).length == 0);
    
    graph.addGroup("node-group");
    
    expect.assert(graph.hasGroup("node-group"));
    expect.assert(graph.getGroup("node-group") != undefined);
    expect.assert(Object.keys(graph.getGroups()).length == 1);

    graph.removeGroup("node-group");

    expect.assert(!graph.hasGroup("node-group"));
    expect.assert(graph.getGroup("node-group") == undefined);
    expect.assert(Object.keys(graph.getGroups()).length == 0);
  });

  it("test_NodeGraph_specific_metadata", function() {
    var loader: ModuleLoader = new ModuleLoaderNodeJSPath();
    var nodeModule: NodeModule = loader.newModule("anonymous");
    var graph: NodeGraph = nodeModule.addGraph("graph");

    var metadata: Metadata = graph.getMetadata();

    // Check that we don't have any specific metadata yet.
    expect.assert(!metadata.hasMetadata("foo.bar.org"));

    // Check that when we ask for metadata, it's created
    var cd: ConnectionData = metadata.getMetadata("foo.bar.org");
    expect.assert(metadata.hasMetadata("foo.bar.org"));

    cd.setValue("x", "some-value");

    // Javascript/TypeScript has no such thing as const-ness, so
    // we don't worry about the rest of this test
    // Check that we can make a const-version of this.
    // Check that we can make a const-version of this.
    //const NodeGraph * constGraph = graph;
    //const Metadata & constMetadata = constGraph.getMetadata();
    //const ConnectionData & constData = constMetadata.getMetadata("foo.bar.org");
    //expect.assert(constData.hasValue("x"));
    //
    //const ConnectionData & readonly = constMetadata.getMetadata("some-other-namespace");
    //// Check that in the const context, we didn't actually add the namespace,
    //// just faked it by returning an empty connection data.
    //expect.assert(!constMetadata.hasMetadata("some-other-namespace"));
    //expect.assert(!readonly.hasValue("x"));
  });
});
