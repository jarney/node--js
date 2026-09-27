#include <catch2/catch_all.hpp>

#include "node--js/NodeType.hpp"
#include "node--js/NodeModule.hpp"

using namespace NodeJS::core;

TEST_CASE("NodeModule main", "[NodeJS][NodeModule]")
{
    NodeModule nodeModule;
    nodeModule.addDataType(std::make_unique<DataType>("abc", "def"));

    auto newNodeType = std::make_unique<NodeType>();
    newNodeType->setId("fooNewNode");
    newNodeType->addInputPort("input0", std::make_unique<NodePort>(*nodeModule.getDataType("abc"), NodePort::ConnectionPolicy::One));
    nodeModule.addNodeType(std::move(newNodeType));

    CHECK(nodeModule.hasDataType("abc"));
    CHECK(nodeModule.hasNodeType("fooNewNode"));
    
}

TEST_CASE("Node Type preserves ID", "[NodeJS][NodeType]")
{
    NodeType nodeType;

    nodeType.setId("foo");
    CHECK(nodeType.getId() == "foo");
    
    fprintf(stderr, "Doing some testing\n");
}
TEST_CASE("Node Type Input Ports", "[NodeJS][NodeType]")
{
    DataType dataType("abc", "def");
    
    NodeType nodeType;
    nodeType.addInputPort("input0", std::make_unique<NodePort>(dataType, NodePort::ConnectionPolicy::One));
    CHECK(nodeType.getInputPortCount() == 1);
    CHECK(nodeType.hasInputPort("input0"));
    CHECK(!nodeType.hasOutputPort("input0"));
}
