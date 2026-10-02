#include <catch2/catch_all.hpp>

#include "node--js/NodeType.hpp"

using namespace NodeJS::core;

TEST_CASE("NodeType empty", "[NodeJS][core][NodeType]")
{
    NodeType nodeType;

    nodeType.setId("add");
    nodeType.setVisibility(NodeType::Visibility::PRIVATE);
    nodeType.setType(NodeType::Type::NATIVE);

    CHECK(nodeType.getVisibility() == NodeType::Visibility::PRIVATE);
    CHECK(nodeType.getType() == NodeType::Type::NATIVE);

    nodeType.setVisibility(NodeType::Visibility::PUBLIC);
    nodeType.setType(NodeType::Type::GRAPH);
    
    CHECK(nodeType.getVisibility() == NodeType::Visibility::PUBLIC);
    CHECK(nodeType.getType() == NodeType::Type::GRAPH);
}


TEST_CASE("NodeType some ports", "[NodeJS][core][NodeType]")
{
    NodeType nodeType;
    nodeType.setId("add");
    nodeType.addInputPort("first", std::make_unique<NodePort>("variable", "First Argument"));
    nodeType.addInputPort("second", std::make_unique<NodePort>("variable", "Second Argument"));
    nodeType.addOutputPort("output", std::make_unique<NodePort>("variable", "Result"));

    CHECK(nodeType.getInputPortCount() == 2);
    CHECK(nodeType.getOutputPortCount() == 1);

    CHECK(nodeType.getInputPortByName("first")->getDescription() == "First Argument");
    CHECK(nodeType.getInputPortName(0) == "first");
    CHECK(nodeType.getInputPortByIndex(0)->getDescription() == "First Argument");
    CHECK(nodeType.hasInputPort("first"));
    
    CHECK(nodeType.getInputPortByName("second")->getDescription() == "Second Argument");
    CHECK(nodeType.getInputPortName(1) == "second");
    CHECK(nodeType.getInputPortByIndex(1)->getDescription() == "Second Argument");

    CHECK(nodeType.getOutputPortByName("output")->getDescription() == "Result");
    CHECK(nodeType.getOutputPortName(0) == "output");
    CHECK(nodeType.getOutputPortByIndex(0)->getDescription() == "Result");
    CHECK(nodeType.hasOutputPort("output"));

    // Now, some edge cases:
    CHECK(nodeType.getInputPortByName("non-existent") == nullptr);
    CHECK(nodeType.getOutputPortByName("non-existent") == nullptr);
    CHECK(!nodeType.hasInputPort("non-existent"));
    CHECK(!nodeType.hasOutputPort("non-existent"));
    CHECK(nodeType.getInputPortName(99) == "");
    CHECK(nodeType.getOutputPortName(99) == "");
    CHECK(nodeType.getInputPortByIndex(99) == nullptr);
    CHECK(nodeType.getOutputPortByIndex(1) == nullptr);
}

TEST_CASE("NodeType unique input ports", "[NodeJS][core][NodeType]")
{
    NodeType nodeType;
    bool rc1 = nodeType.addInputPort("first", std::make_unique<NodePort>("variable", "First Argument"));
    bool rc2 = nodeType.addInputPort("first", std::make_unique<NodePort>("variable", "Another"));

    CHECK(rc1 == true);
    CHECK(rc2 == false);

    CHECK(nodeType.getInputPortCount() == 1);
    CHECK(nodeType.getOutputPortCount() == 0);
    CHECK(nodeType.getInputPortByName("first")->getDescription() == "First Argument");
}

TEST_CASE("NodeType unique output ports", "[NodeJS][core][NodeType]")
{
    NodeType nodeType;
    bool rc1 = nodeType.addOutputPort("first", std::make_unique<NodePort>("variable", "First Argument"));
    bool rc2 = nodeType.addOutputPort("first", std::make_unique<NodePort>("variable", "Another"));

    CHECK(rc1 == true);
    CHECK(rc2 == false);

    CHECK(nodeType.getInputPortCount() == 0);
    CHECK(nodeType.getOutputPortCount() == 1);
    CHECK(nodeType.getOutputPortByName("first")->getDescription() == "First Argument");
}

