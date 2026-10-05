#include <catch2/catch_all.hpp>

#include "node--js/NodeType.hpp"
#include "node--js/NodeModule.hpp"
#include "node--js/xml/ModuleLoaderNodeJSPath.hpp"

using namespace NodeJS::core;
using namespace NodeJS::xml;

TEST_CASE("Node Module package id", "[NodeJS][core][NodeModule]")
{
    ModuleLoaderNodeJSPath loader;
    NodeModule & nodeModule = *loader.newModule("anonymous");

    nodeModule.setPackage("simple.package.name");

    CHECK(nodeModule.getPackage() == "simple.package.name");
}

TEST_CASE("NodeModule single type", "[NodeJS][core][NodeModule]")
{
    ModuleLoaderNodeJSPath loader;
    NodeModule & nodeModule = *loader.newModule("anonymous");
    
    auto nodeType = std::make_unique<NodeType>();
    nodeType->setId("add");
    nodeType->getInputs().addPort("first", std::make_unique<NodePort>("variable", "First Argument"));
    
    nodeModule.addNodeType(std::move(nodeType));

    CHECK(nodeModule.hasNodeType("add"));
    
    const NodeType *nt = nodeModule.getNodeType("add");
    
    CHECK(nt->getId() == "add");
    CHECK(nt->getInputs().getCount() == 1);

    const NodeType *nonexistent = nodeModule.getNodeType("invalid-node");
    CHECK(nonexistent == nullptr);

    nodeModule.removeNodeType("add");
    CHECK(!nodeModule.hasNodeType("add"));
}

TEST_CASE("NodeModule Iterator", "[NodeJS][core][NodeModule]")
{
    ModuleLoaderNodeJSPath loader;
    NodeModule & nodeModule = *loader.newModule("anonymous");
    
    auto nodeType = std::make_unique<NodeType>();
    nodeType->setId("add");
    nodeType->getInputs().addPort("first", std::make_unique<NodePort>("variable", "First Argument"));
    nodeModule.addNodeType(std::move(nodeType));

    int i = 0;
    for (const auto & it : nodeModule.getNodeTypes()) {
	CHECK(it.first == "add");
	i++;
    }
    // Check that we actually iterated.
    CHECK(i == 1);
}

TEST_CASE("NodeModule Check basic data type stuff", "[NodeJS][core][NodeModule]")
{
    ModuleLoaderNodeJSPath loader;
    NodeModule & nodeModule = *loader.newModule("anonymous");

    auto dataType = std::make_unique<DataType>("variable", "Variable Type");
    nodeModule.addDataType(std::move(dataType));

    CHECK(nodeModule.hasDataType("variable"));
    CHECK(nodeModule.getDataType("variable")->getName() == "Variable Type");

    nodeModule.removeDataType("variable");

    CHECK(!nodeModule.hasDataType("variable"));
    CHECK(nodeModule.getDataType("variable") == nullptr);
    
}

TEST_CASE("NodeModule Data Type Iterator", "[NodeJS][core][NodeModule]")
{
    ModuleLoaderNodeJSPath loader;
    NodeModule & nodeModule = *loader.newModule("anonymous");
    
    auto dataType = std::make_unique<DataType>("variable", "Variable Type");
    nodeModule.addDataType(std::move(dataType));

    int i = 0;
    for (const auto & it : nodeModule.getDataTypes()) {
	CHECK(it.first == "variable");
	CHECK(it.second->getId() == "variable");
	i++;
    }
    // Check that we actually iterated.
    CHECK(i == 1);
}
