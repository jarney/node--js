#include <catch2/catch_all.hpp>

#include "node--js/NodeType.hpp"
#include "node--js/NodeModule.hpp"
#include "node--js/xml/ModuleLoaderNodeJSPath.hpp"

using namespace NodeJS::core;
using namespace NodeJS::xml;

TEST_CASE("NodeModule main", "[NodeJS][core][NodeModule]")
{
    ModuleLoaderNodeJSPath loader;
    NodeModule & nodeModule = *loader.newModule("anonymous");
    nodeModule.addDataType(std::make_unique<DataType>("abc", "def"));

    auto newNodeType = std::make_unique<NodeType>();
    newNodeType->setId("fooNewNode");
    newNodeType->getInputs().addPort("input0", std::make_unique<NodePort>("abc", "Something Wild", NodePort::ConnectionPolicy::One));
    nodeModule.addNodeType(std::move(newNodeType));

    CHECK(nodeModule.hasDataType("abc"));
    CHECK(nodeModule.hasNodeType("fooNewNode"));
    
}

TEST_CASE("Node Type preserves ID", "[NodeJS][core][NodeType]")
{
    NodeType nodeType;

    nodeType.setId("foo");
    CHECK(nodeType.getId() == "foo");
    
    fprintf(stderr, "Doing some testing\n");
}

TEST_CASE("Node Type Input Ports", "[NodeJS][core][NodeType]")
{
    NodeType nodeType;
    nodeType.getInputs().addPort("input0", std::make_unique<NodePort>("abc", "Something Wild", NodePort::ConnectionPolicy::One));
    CHECK(nodeType.getInputs().getCount() == 1);
    CHECK(nodeType.getInputs().hasPort("input0"));
    CHECK(!nodeType.getOutputs().hasPort("input0"));
}
