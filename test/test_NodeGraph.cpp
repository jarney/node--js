#include <catch2/catch_all.hpp>

#include "node--js/NodeGraph.hpp"
#include "node--js/NodeModule.hpp"
#include "node--js/xml/ModuleLoaderNodeJSPath.hpp"

using namespace NodeJS::core;
using namespace NodeJS::xml;

TEST_CASE("test_NodeGraph_create_node", "[NodeJS][core][NodeGraph][Node]")
{
    ModuleLoaderNodeJSPath loader;
    NodeModule & nodeModule = *loader.newModule("anonymous");
    NodeGraph *graph = nodeModule.addGraph("graph");;
    ConnectionData nodeData;
    NodeType nodeType;

    // Check that our id generation scheme
    // works to prevent clashes by incrementing
    // a number if we accidentally clash.  This is a 'freecad-like'
    // scheme where it automatically numbers elements if we clash.
    Node & createdNode = graph->newNode(nodeType, "main", nodeData);
    CHECK(createdNode.getId() == "main");

    Node & nextNode0 = graph->newNode(nodeType, "main", nodeData);
    CHECK(nextNode0.getId() == "main-0");

    Node & nextNode1 = graph->newNode(nodeType, "main", nodeData);
    CHECK(nextNode1.getId() == "main-1");

}

TEST_CASE("test_NodeGraph_registered_node_type", "[NodeJS][core][NodeGraph][Node]")
{

    ModuleLoaderNodeJSPath loader;
    NodeModule & nodeModule = *loader.newModule("anonymous");
    NodeGraph *graph = nodeModule.addGraph("graph");;
    ConnectionData nodeData;
    NodeType nodeType;

    // Check that the node type always refers to the
    // same node type we gave it to begin with.  Nodes don't
    // carry their own copies of the node data, but instead
    // just refer to the node type you gave it, making
    // the caller responsible for the node type's lifetime.
    
    Node & createdNode = graph->newNode(nodeType, "main", nodeData);

    CHECK(&createdNode.getType() == &nodeType);
    CHECK(&createdNode.getGraph() == graph);
}

TEST_CASE("test_NodeGraph_copied_data", "[NodeJS][core][NodeGraph][Node]")
{

    ModuleLoaderNodeJSPath loader;
    NodeModule & nodeModule = *loader.newModule("anonymous");
    NodeGraph *graph = nodeModule.addGraph("graph");;
    ConnectionData nodeData;
    NodeType nodeType;

    nodeData.setValue("initial-value", "some-data");
    Node & createdNode = graph->newNode(nodeType, "main", nodeData);

    // Check that our node's data was initialized with the data
    // we gave it.
    CHECK(createdNode.getData().hasValue("initial-value"));

    // Check that our node's data is a copy and doesn't
    // refer to the same data.
    createdNode.getData().setValue("runtime-value", "another one");
    CHECK(createdNode.getData().hasValue("runtime-value"));
    CHECK(!nodeData.hasValue("runtime-value"));

    nodeData.setValue("runtime-for-initializer", "initializer");
    CHECK(!createdNode.getData().hasValue("runtime-for-initializer"));
    CHECK(nodeData.hasValue("runtime-for-initializer"));
}

TEST_CASE("test_NodeGraph_node_existence", "[NodeJS][core][NodeGraph][Node]")
{
    ModuleLoaderNodeJSPath loader;
    NodeModule & nodeModule = *loader.newModule("anonymous");
    NodeGraph *graph = nodeModule.addGraph("graph");;
    ConnectionData nodeData;
    NodeType nodeType;
    
    Node & createdNode = graph->newNode(nodeType, "main", nodeData);

    CHECK(graph->hasNode("main"));

    // Check that we got back the same
    // node as when we created it.
    Node* retrieved = graph->getNode("main");
    CHECK(retrieved == &createdNode);

    // Check that we get the right
    // behavior for non-existing nodes.
    CHECK(!graph->hasNode("something-else"));
    CHECK(graph->getNode("something-else") == nullptr);
}

TEST_CASE("test_NodeGraph_create_edge", "[NodeJS][core][NodeGraph][Edge]")
{
    ModuleLoaderNodeJSPath loader;
    NodeModule & nodeModule = *loader.newModule("anonymous");
    NodeGraph *graph = nodeModule.addGraph("graph");;
    std::optional<EdgeId> edgeId = graph->newEdge("a", "first", "b", "second");
    CHECK(edgeId.has_value());
}

TEST_CASE("test_NodeGraph_edge_duplicate", "[NodeJS][core][NodeGraph][Edge]")
{
    ModuleLoaderNodeJSPath loader;
    NodeModule & nodeModule = *loader.newModule("anonymous");
    NodeGraph *graph = nodeModule.addGraph("graph");;
    std::optional<EdgeId> edgeId = graph->newEdge("a", "first", "b", "second");
    std::optional<EdgeId> edgeId2 = graph->newEdge("a", "first", "b", "second");
    CHECK(edgeId.has_value());
    CHECK(!edgeId2.has_value());
}
TEST_CASE("test_NodeGraph_topological_sort", "[NodeJS][core][NodeGraph][Algorithms]")
{
    ModuleLoaderNodeJSPath loader;
    NodeModule & nodeModule = *loader.newModule("anonymous");
    NodeGraph &graph = *nodeModule.addGraph("main");
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
    CHECK(maybeNodesSorted.has_value());

    std::vector<NodeId> nodesSorted = maybeNodesSorted.value();
    std::vector<NodeId> correctOrder = {"A", "E", "B", "C"};
    std::vector<NodeId> incorrectOrder = {"C", "B", "A", "E"};
    CHECK(nodesSorted == correctOrder);
    CHECK(nodesSorted != incorrectOrder);
}

TEST_CASE("test_NodeGraph_topological_sort_cycle_detection", "[NodeJS][core][NodeGraph][Algorithms]")
{
    ModuleLoaderNodeJSPath loader;
    NodeModule & nodeModule = *loader.newModule("anonymous");
    NodeGraph &graph = *nodeModule.addGraph("main");
    NodeType plain;
    
    graph.newNode(plain, "C");
    graph.newNode(plain, "B");
    graph.newNode(plain, "A");
    
    graph.newEdge("A", "x", "B", "x");
    graph.newEdge("B", "x", "C", "x");
    graph.newEdge("C", "x", "A", "x");
    
    std::optional<std::vector<NodeId>> maybeNodesSorted = graph.getNodeIdsInTopologicalOrder();
    CHECK(!maybeNodesSorted.has_value());    
}

TEST_CASE("test_NodeGraph_getModule", "[NodeJS][core][NodeGraph][Node]")
{
    ModuleLoaderNodeJSPath loader;
    NodeModule & nodeModule = *loader.newModule("anonymous");
    NodeGraph & graph = *nodeModule.addGraph("graph");

    CHECK(&nodeModule == &graph.getModule());

}

TEST_CASE("test_NodeGraph_erase_node", "[NodeJS][core][NodeGraph][Node]")
{
    ModuleLoaderNodeJSPath loader;
    NodeModule & nodeModule = *loader.newModule("anonymous");
    NodeGraph & graph = *nodeModule.addGraph("graph");
    NodeType nodeType;

    graph.newNode(nodeType, "main");

    CHECK(graph.hasNode("main"));
    
    graph.removeNode("main");

    CHECK(!graph.hasNode("main"));
}

TEST_CASE("test_NodeGraph_erase_node_with_edges", "[NodeJS][core][NodeGraph][Node]")
{
    ModuleLoaderNodeJSPath loader;
    NodeModule & nodeModule = *loader.newModule("anonymous");
    NodeGraph & graph = *nodeModule.addGraph("graph");
    NodeType nodeType;

    graph.newNode(nodeType, "a");
    graph.newNode(nodeType, "b");

    CHECK(graph.hasNode("a"));
    CHECK(graph.hasNode("b"));

    graph.newEdge("a", "x", "b", "x");
    graph.newEdge("b", "x", "a", "x");
    
    CHECK(graph.getEdgesFrom("a").size() == 1);
    CHECK(graph.getEdgesTo("a").size() == 1);
    CHECK(graph.getEdgesFrom("b").size() == 1);
    CHECK(graph.getEdgesTo("b").size() == 1);

    graph.removeNode("a");

    CHECK(!graph.hasNode("a"));

    CHECK(graph.getEdgesFrom("a").size() == 0);
    CHECK(graph.getEdgesTo("a").size() == 0);

    CHECK(graph.getEdgesFrom("b").size() == 0);
    CHECK(graph.getEdgesTo("b").size() == 0);

    CHECK(graph.getEdges().size() == 0);
}

TEST_CASE("test_NodeGraph_erase_node_with_extra_edges", "[NodeJS][core][NodeGraph][Node]")
{
    ModuleLoaderNodeJSPath loader;
    NodeModule & nodeModule = *loader.newModule("anonymous");
    NodeGraph & graph = *nodeModule.addGraph("graph");
    NodeType nodeType;

    graph.newNode(nodeType, "a");
    graph.newNode(nodeType, "b");
    graph.newNode(nodeType, "c");
    graph.newNode(nodeType, "d");

    CHECK(graph.hasNode("a"));
    CHECK(graph.hasNode("b"));

    graph.newEdge("a", "x", "b", "x");
    graph.newEdge("b", "x", "a", "x");

    graph.newEdge("a", "x", "c", "x");
    graph.newEdge("b", "x", "d", "x");

    graph.newEdge("c", "x", "d", "x");
    graph.newEdge("d", "x", "c", "x");
    
    CHECK(graph.getEdgesFrom("a").size() == 2);
    CHECK(graph.getEdgesTo("a").size() == 1);
    CHECK(graph.getEdgesFrom("b").size() == 2);
    CHECK(graph.getEdgesTo("b").size() == 1);

    graph.removeNode("a");

    CHECK(!graph.hasNode("a"));

    CHECK(graph.getEdgesFrom("a").size() == 0);
    CHECK(graph.getEdgesTo("a").size() == 0);

    CHECK(graph.getEdgesFrom("b").size() == 1);
    CHECK(graph.getEdgesTo("b").size() == 0);

    CHECK(graph.getEdges().size() == 3);
}

TEST_CASE("test_NodeGraph_erase_single_edge", "[NodeJS][core][NodeGraph][Node]")
{
    ModuleLoaderNodeJSPath loader;
    NodeModule & nodeModule = *loader.newModule("anonymous");
    NodeGraph & graph = *nodeModule.addGraph("graph");
    NodeType nodeType;

    graph.newNode(nodeType, "a");
    graph.newNode(nodeType, "b");
    graph.newNode(nodeType, "c");

    CHECK(graph.hasNode("a"));
    CHECK(graph.hasNode("b"));

    graph.newEdge("a", "x", "b", "x");
    graph.newEdge("a", "p", "c", "q");
    graph.newEdge("c", "p", "b", "q");

    CHECK(graph.getEdgesFrom("a").size() == 2);
    CHECK(graph.getEdgesTo("b").size() == 2);

    graph.removeEdge("a", "x", "b", "x");

    CHECK(graph.getEdgesFrom("a").size() == 1);
    CHECK(graph.getEdgesTo("b").size() == 1);
}

TEST_CASE("test_NodeGraph_search_scopes", "[NodeJS][core][NodeGraph][Node]")
{
    ModuleLoaderNodeJSPath loader;
    NodeModule & nodeModule = *loader.newModule("anonymous");
    NodeGraph & graph = *nodeModule.addGraph("graph");

    NodeModule & typesScope = *loader.newModule("where-we-define-the-types");
    auto nodeType1 = std::make_unique<NodeType>();
    nodeType1->setId("type1-node");
    typesScope.addNodeType(std::move(nodeType1));

    auto dataType1 = std::make_unique<DataType>("foo", "bar");
    typesScope.addDataType(std::move(dataType1));

    // Before we add the search scope,
    // we cannot resolve this type.
    CHECK(graph.getNodeType("type1-node") == nullptr);
    CHECK(graph.getDataType("foo") == nullptr);

    // Add nodeModule to our search scope.
    graph.addScope(&typesScope);

    // After we add the scope, we can find it.
    CHECK(graph.getNodeType("type1-node") != nullptr);
    CHECK(graph.getDataType("foo") != nullptr);

    NodeGraph & graphCopy = *nodeModule.addGraph("graph-copy");

    // Before we copy the scope, we can't resolve it.
    CHECK(graphCopy.getNodeType("type1-node") == nullptr);
    CHECK(graphCopy.getDataType("foo") == nullptr);

    graphCopy.copyScope(&graph);

    // After copying the scopes, we can resolve the names.
    CHECK(graphCopy.getNodeType("type1-node") != nullptr);
    CHECK(graphCopy.getDataType("foo") != nullptr);

    
}

TEST_CASE("test_NodeGraph_groups", "[NodeJS][core][NodeGraph][Node]")
{
    ModuleLoaderNodeJSPath loader;
    NodeModule & nodeModule = *loader.newModule("anonymous");
    NodeGraph & graph = *nodeModule.addGraph("graph");

    // Check that basic add and remove works.
    
    CHECK(!graph.hasGroup("node-group"));
    CHECK(graph.getGroup("node-group") == nullptr);
    CHECK(graph.getGroups().size() == 0);
    
    graph.addGroup("node-group");
    
    CHECK(graph.hasGroup("node-group"));
    CHECK(graph.getGroup("node-group") != nullptr);
    CHECK(graph.getGroups().size() == 1);

    graph.removeGroup("node-group");

    CHECK(!graph.hasGroup("node-group"));
    CHECK(graph.getGroup("node-group") == nullptr);
    CHECK(graph.getGroups().size() == 0);
}

TEST_CASE("test_NodeGraph_specific_metadata", "[NodeJS][core][NodeGraph][Node]")
{
    ModuleLoaderNodeJSPath loader;
    NodeModule & module = *loader.newModule("anonymous");
    NodeGraph *graph = module.addGraph("graph");

    Metadata & metadata = graph->getMetadata();

    // Check that we don't have any specific metadata yet.
    CHECK(!metadata.hasMetadata("foo.bar.org"));

    // Check that when we ask for metadata, it's created
    ConnectionData & cd = metadata.getMetadata("foo.bar.org");
    CHECK(metadata.hasMetadata("foo.bar.org"));

    cd.setValue("x", "some-value");

    // Check that we can make a const-version of this.
    const NodeGraph * constGraph = graph;
    const Metadata & constMetadata = constGraph->getMetadata();
    const ConnectionData & constData = constMetadata.getMetadata("foo.bar.org");
    CHECK(constData.hasValue("x"));

    const ConnectionData & readonly = constMetadata.getMetadata("some-other-namespace");
    // Check that in the const context, we didn't actually add the namespace,
    // just faked it by returning an empty connection data.
    CHECK(!constMetadata.hasMetadata("some-other-namespace"));
    CHECK(!readonly.hasValue("x"));
}
