#include <catch2/catch_all.hpp>

#include "node--js/NodeGraph.hpp"

using namespace NodeJS::core;

TEST_CASE("NodeGraph create node", "[NodeJS][core][NodeGraph][Node]")
{

    NodeGraph graph;
    ConnectionData nodeData;
    NodeType nodeType;

    // Check that our id generation scheme
    // works to prevent clashes by incrementing
    // a number if we accidentally clash.  This is a 'freecad-like'
    // scheme where it automatically numbers elements if we clash.
    Node & createdNode = graph.newNode(nodeType, "main", nodeData);
    CHECK(createdNode.getId() == "main");

    Node & nextNode0 = graph.newNode(nodeType, "main", nodeData);
    CHECK(nextNode0.getId() == "main-0");

    Node & nextNode1 = graph.newNode(nodeType, "main", nodeData);
    CHECK(nextNode1.getId() == "main-1");

}

TEST_CASE("NodeGraph registered node type", "[NodeJS][core][NodeGraph][Node]")
{

    NodeGraph graph;
    ConnectionData nodeData;
    NodeType nodeType;

    // Check that the node type always refers to the
    // same node type we gave it to begin with.  Nodes don't
    // carry their own copies of the node data, but instead
    // just refer to the node type you gave it, making
    // the caller responsible for the node type's lifetime.
    
    Node & createdNode = graph.newNode(nodeType, "main", nodeData);

    CHECK(&createdNode.getType() == &nodeType);
    CHECK(&createdNode.getGraph() == &graph);
}

TEST_CASE("NodeGraph copied data", "[NodeJS][core][NodeGraph][Node]")
{

    NodeGraph graph;
    ConnectionData nodeData;
    NodeType nodeType;

    nodeData.setValue("initial-value", "some-data");
    Node & createdNode = graph.newNode(nodeType, "main", nodeData);

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

TEST_CASE("NodeGraph node existence", "[NodeJS][core][NodeGraph][Node]")
{
    NodeGraph graph;
    ConnectionData nodeData;
    NodeType nodeType;
    
    Node & createdNode = graph.newNode(nodeType, "main", nodeData);

    CHECK(graph.hasNode("main"));

    // Check that we got back the same
    // node as when we created it.
    Node* retrieved = graph.getNode("main");
    CHECK(retrieved == &createdNode);

    // Check that we get the right
    // behavior for non-existing nodes.
    CHECK(!graph.hasNode("something-else"));
    CHECK(graph.getNode("something-else") == nullptr);
}

TEST_CASE("NodeGraph create edge", "[NodeJS][core][NodeGraph][Edge]")
{
    NodeGraph graph;
    std::optional<EdgeId> edgeId = graph.newEdge("a", "first", "b", "second");
    CHECK(edgeId.has_value());
}

TEST_CASE("NodeGraph edge duplicate", "[NodeJS][core][NodeGraph][Edge]")
{
    NodeGraph graph;
    std::optional<EdgeId> edgeId = graph.newEdge("a", "first", "b", "second");
    std::optional<EdgeId> edgeId2 = graph.newEdge("a", "first", "b", "second");
    CHECK(edgeId.has_value());
    CHECK(!edgeId2.has_value());
}
