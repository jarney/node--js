#include <catch2/catch_all.hpp>

#include "node--js/NodeType.hpp"
#include "node--js/NodeModule.hpp"
#include "node--js/xml/ModuleLoaderNodeJSPath.hpp"

using namespace NodeJS::core;
using namespace NodeJS::xml;

TEST_CASE("test_NodeModule_package_id", "[NodeJS][core][NodeModule]")
{
    ModuleLoaderNodeJSPath loader;
    NodeModule & nodeModule = *loader.newModule("anonymous");

    nodeModule.setPackage("simple.package.name");
    nodeModule.setDescription("Some Desc");

    CHECK(nodeModule.getPackage() == "simple.package.name");
    CHECK(nodeModule.getDescription() == "Some Desc");
}

TEST_CASE("test_NodeModule_single_type", "[NodeJS][core][NodeModule]")
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

    // This is actually really dangerous and maybe we should
    // not even have such a method because it would
    // orphan actual nodes in graphs.
    nodeModule.removeNodeType("add");
    CHECK(!nodeModule.hasNodeType("add"));
}

TEST_CASE("test_NodeModule_iterator", "[NodeJS][core][NodeModule]")
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

TEST_CASE("test_NodeModule_check_basic_data_type_stuff", "[NodeJS][core][NodeModule]")
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

TEST_CASE("test_NodeModule_data_type_iterator", "[NodeJS][core][NodeModule]")
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

TEST_CASE("test_NodeModule_getModuleLoader", "[NodeJS][core][NodeModule]")
{
    ModuleLoaderNodeJSPath loader;
    NodeModule & nodeModule = *loader.newModule("anonymous");

    CHECK(&loader == &nodeModule.getModuleLoader());
}

TEST_CASE("test_NodeModule_add_graph_uniqueness_conditions", "[NodeJS][core][NodeModule]")
{
    ModuleLoaderNodeJSPath loader;
    NodeModule & nodeModule = *loader.newModule("anonymous");

    NodeGraph *graph1 = nodeModule.addGraph("graph");
    NodeGraph *graph1_out = nodeModule.getGraph("graph");
    CHECK(graph1 == graph1_out);

    NodeGraph *graph2 = nodeModule.addGraph("graph");

    CHECK(graph1 != nullptr);
    CHECK(graph2 == nullptr);

    CHECK(nodeModule.getGraph("nonexistent") == nullptr);
}
