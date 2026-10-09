#include <catch2/catch_all.hpp>

#include "node--js/NodeType.hpp"

using namespace NodeJS::core;

TEST_CASE("test_NodeType_empty", "[NodeJS][core][NodeType]")
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


TEST_CASE("test_NodeType_some_ports", "[NodeJS][core][NodeType]")
{
    NodeType nodeType;
    nodeType.setId("add");
    nodeType.getInputs().addPort("first", std::make_unique<NodePort>("variable", "First Argument"));
    nodeType.getInputs().addPort("second", std::make_unique<NodePort>("variable", "Second Argument"));
    nodeType.getOutputs().addPort("output", std::make_unique<NodePort>("variable", "Result"));

    CHECK(nodeType.getInputs().getCount() == 2);
    CHECK(nodeType.getOutputs().getCount() == 1);

    CHECK(nodeType.getInputs().getByName("first")->getDescription() == "First Argument");
    CHECK(nodeType.getInputs().getName(0) == "first");
    CHECK(nodeType.getInputs().getByIndex(0)->getDescription() == "First Argument");
    CHECK(nodeType.getInputs().hasPort("first"));
    
    CHECK(nodeType.getInputs().getByName("second")->getDescription() == "Second Argument");
    CHECK(nodeType.getInputs().getName(1) == "second");
    CHECK(nodeType.getInputs().getByIndex(1)->getDescription() == "Second Argument");

    CHECK(nodeType.getOutputs().getByName("output")->getDescription() == "Result");
    CHECK(nodeType.getOutputs().getName(0) == "output");
    CHECK(nodeType.getOutputs().getByIndex(0)->getDescription() == "Result");
    CHECK(nodeType.getOutputs().hasPort("output"));

    // Now, some edge cases:
    CHECK(nodeType.getInputs().getByName("non-existent") == nullptr);
    CHECK(nodeType.getOutputs().getByName("non-existent") == nullptr);
    CHECK(!nodeType.getInputs().hasPort("non-existent"));
    CHECK(!nodeType.getOutputs().hasPort("non-existent"));
    CHECK(nodeType.getInputs().getName(99) == "");
    CHECK(nodeType.getOutputs().getName(99) == "");
    CHECK(nodeType.getInputs().getByIndex(99) == nullptr);
    CHECK(nodeType.getOutputs().getByIndex(1) == nullptr);
}

TEST_CASE("test_NodeType_unique_input_ports", "[NodeJS][core][NodeType]")
{
    NodeType nodeType;
    bool rc1 = nodeType.getInputs().addPort("first", std::make_unique<NodePort>("variable", "First Argument"));
    bool rc2 = nodeType.getInputs().addPort("first", std::make_unique<NodePort>("variable", "Another"));

    CHECK(rc1 == true);
    CHECK(rc2 == false);

    CHECK(nodeType.getInputs().getCount() == 1);
    CHECK(nodeType.getOutputs().getCount() == 0);
    CHECK(nodeType.getInputs().getByName("first")->getDescription() == "First Argument");
}

TEST_CASE("test_NodeType_unique_output_ports", "[NodeJS][core][NodeType]")
{
    NodeType nodeType;
    bool rc1 = nodeType.getOutputs().addPort("first", std::make_unique<NodePort>("variable", "First Argument"));
    bool rc2 = nodeType.getOutputs().addPort("first", std::make_unique<NodePort>("variable", "Another"));

    CHECK(rc1 == true);
    CHECK(rc2 == false);

    CHECK(nodeType.getInputs().getCount() == 0);
    CHECK(nodeType.getOutputs().getCount() == 1);
    CHECK(nodeType.getOutputs().getByName("first")->getDescription() == "First Argument");
}

TEST_CASE("test_NodeType_default_node_data", "[NodeJS][core][NodeType]")
{
    NodeType nodeType;

    ConnectionData defaultData;
    defaultData.setValue("x", "foo");
    nodeType.setDefaultNodeData(defaultData);

    CHECK(nodeType.getDefaultNodeData().getValue("x") == "foo");
    
}
