#include <catch2/catch_all.hpp>

#include "node--js/NodeGraph.hpp"
#include "node--js/NodeModule.hpp"
#include "node--js/Node.hpp"
#include "node--js/xml/ModuleLoaderNodeJSPath.hpp"

using namespace NodeJS::core;
using namespace NodeJS::xml;

static const char *DATA_TYPE = "variable";

TEST_CASE("Node create node", "[NodeJS][core][NodeGraph][Node]")
{
    ModuleLoaderNodeJSPath loader;
    NodeModule & module = *loader.newModule("anonymous");
    NodeGraph *graph = module.addGraph("graph");;

    NodeType nodeType;

    // First, just check identity stuff.
    Node & createdNode = graph->newNode(nodeType, "main");
    CHECK(createdNode.getId() == "main");
    CHECK(&createdNode.getType() == &nodeType);
}

TEST_CASE("Node type i/o", "[NodeJS][core][NodeGraph][Node]")
{
    ModuleLoaderNodeJSPath loader;
    NodeModule & module = *loader.newModule("anonymous");
    NodeGraph *graph = module.addGraph("graph");

    NodeType nodeType;
    nodeType.getInputs().addPort("a", std::make_unique<NodePort>(DATA_TYPE, "adesc", NodePort::ConnectionPolicy::Multiple));
    nodeType.getInputs().addPort("b", std::make_unique<NodePort>(DATA_TYPE, "bdesc", NodePort::ConnectionPolicy::Multiple));
    nodeType.getOutputs().addPort("out", std::make_unique<NodePort>(DATA_TYPE, "out", NodePort::ConnectionPolicy::Multiple));

    Node & node = graph->newNode(nodeType, "main");

    CHECK(node.getType().getInputs().getCount() == 2);
    CHECK(node.getType().getOutputs().getCount() == 1);

    // So we can validate that the node type's data is correct,
    // but we should also verify the node-specific port info.
    CHECK(node.getType().getInputs().getByName("a")->getDescription() == "adesc");
    CHECK(node.getType().getInputs().getByName("b")->getDescription() == "bdesc");
    CHECK(node.getType().getOutputs().getByName("out")->getDescription() == "out");

    CHECK(node.getInputs().getByName("a")->getDescription() == "adesc");
    CHECK(node.getInputs().getByName("b")->getDescription() == "bdesc");
    CHECK(node.getOutputs().getByName("out")->getDescription() == "out");
}

TEST_CASE("Node specific i/o", "[NodeJS][core][NodeGraph][Node]")
{
    ModuleLoaderNodeJSPath loader;
    NodeModule & module = *loader.newModule("anonymous");
    NodeGraph *graph = module.addGraph("graph");

    NodeType nodeType;

    Node & node = graph->newNode(nodeType, "main");
    
    node.getOverrideInputs().addPort("a", std::make_unique<NodePort>(DATA_TYPE, "adesc", NodePort::ConnectionPolicy::Multiple));
    node.getOverrideInputs().addPort("b", std::make_unique<NodePort>(DATA_TYPE, "bdesc", NodePort::ConnectionPolicy::Multiple));
    node.getOverrideOutputs().addPort("out", std::make_unique<NodePort>(DATA_TYPE, "out", NodePort::ConnectionPolicy::Multiple));

    CHECK(!node.hasOverrideInputs());
    CHECK(!node.hasOverrideOutputs());

    // Check that we've set the override ports
    CHECK(node.getOverrideInputs().getCount() == 2);
    CHECK(node.getOverrideOutputs().getCount() == 1);

    // Check that they have not yet taken effect.
    CHECK(node.getInputs().getCount() == 0);
    CHECK(node.getOutputs().getCount() == 0);

    // When we set the node to override, the port counts
    // should change based on what we set up.
    node.setOverrideInputs(true);
    node.setOverrideOutputs(true);

    CHECK(node.hasOverrideInputs());
    CHECK(node.hasOverrideOutputs());
    CHECK(node.getInputs().getCount() == 2);
    CHECK(node.getOutputs().getCount() == 1);
}

TEST_CASE("Node initialize with data", "[NodeJS][core][NodeGraph][Node]")
{
    ModuleLoaderNodeJSPath loader;
    NodeModule & module = *loader.newModule("anonymous");
    NodeGraph *graph = module.addGraph("graph");

    NodeType nodeType;
    ConnectionData initialData;
    initialData.setValue("x", "some-initial-value");
    
    Node & node = graph->newNode(nodeType, "main", initialData);

    // Verify that we initialized the data
    CHECK(node.getData().getValue("x") == "some-initial-value");

    // Verify that if we change the initial data, the node doesn't change
    initialData.setValue("x", "changed our default");
    
    CHECK(initialData.getValue("x") == "changed our default");
    CHECK(node.getData().getValue("x") == "some-initial-value");

    // Verify that if we change the node's data, it
    // is different than the original copy.
    node.getData().setValue("x", "changed node-specific data");
    
    CHECK(initialData.getValue("x") == "changed our default");
    CHECK(node.getData().getValue("x") == "changed node-specific data");

}

TEST_CASE("Node specific metadata", "[NodeJS][core][NodeGraph][Node]")
{
    ModuleLoaderNodeJSPath loader;
    NodeModule & module = *loader.newModule("anonymous");
    NodeGraph *graph = module.addGraph("graph");

    NodeType nodeType;

    Node & node = graph->newNode(nodeType, "main");

    Metadata & metadata = node.getMetadata();

    // Check that we don't have any specific metadata yet.
    CHECK(!metadata.hasMetadata("foo.bar.org"));

    // Check that when we ask for metadata, it's created
    ConnectionData & cd = metadata.getMetadata("foo.bar.org");
    CHECK(metadata.hasMetadata("foo.bar.org"));

    cd.setValue("x", "some-value");

    // Check that we can make a const-version of this.
    const Node & constNode = node;
    const Metadata & constMetadata = constNode.getMetadata();
    const ConnectionData & constData = constMetadata.getMetadata("foo.bar.org");
    CHECK(constData.hasValue("x"));

    const ConnectionData & readonly = constMetadata.getMetadata("some-other-namespace");
    // Check that in the const context, we didn't actually add the namespace,
    // just faked it by returning an empty connection data.
    CHECK(!constMetadata.hasMetadata("some-other-namespace"));
    CHECK(!readonly.hasValue("x"));
}
