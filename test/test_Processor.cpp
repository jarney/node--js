#include <catch2/catch_all.hpp>

#include "node--js/ConnectionData.hpp"
#include "node--js/NodeModule.hpp"
#include "node--js/NodeGraph.hpp"
#include "node--js/NodeType.hpp"
#include "node--js/Processor.hpp"
#include "node--js/xml/ModuleLoaderNodeJSPath.hpp"

using namespace NodeJS::core;
using namespace NodeJS::xml;

class NodeProcessor_binary : public NodeProcessor {
public:
    void process(
	Processor & processor,
	const Node & node,
	const ConnectionData & fromData,
	ConnectionData & toData
	);
};

void
NodeProcessor_binary::process(
    Processor & processor,
    const Node & node,
    const ConnectionData & fromData,
    ConnectionData & toData
    )
{
    toData.setValue("out", "binary call node(" + fromData.getValue("in1") + ", " + fromData.getValue("in2") + ")");
}

class NodeProcessor_unary : public NodeProcessor {
public:
    void process(
	Processor & processor,
	const Node & node,
	const ConnectionData & fromData,
	ConnectionData & toData
	);
};

void
NodeProcessor_unary::process(
    Processor & processor,
    const Node & node,
    const ConnectionData & fromData,
    ConnectionData & toData
    )
{
    toData.setValue("out", "callDataFrom(" + node.getId() + ")");
}

TEST_CASE("Processor minimal", "[NodeJS][core][Processor]")
{
    ModuleLoaderNodeJSPath loader;
    NodeModule & module = *loader.newModule("anonymous");
    NodeGraph & graph = *module.addGraph("graph");;
    ConnectionData nodeData;
    NodeType nodeType;

    NodeType unaryType;
    unaryType.setId("unary");
    unaryType.setType(NodeType::Type::NATIVE);

    NodeType binType;
    binType.setId("binary");
    binType.setType(NodeType::Type::NATIVE);
    
    const Node & main = graph.newNode(binType, "main", nodeData);
    const Node & call1 = graph.newNode(unaryType, "call1", nodeData);
    const Node & call2 = graph.newNode(unaryType, "call2", nodeData);

    graph.newEdge(call1.getId(), "out", main.getId(), "in1");
    graph.newEdge(call2.getId(), "out", main.getId(), "in2");

    Processor processor;
    processor.setNativeImpl("unary", std::make_unique<NodeProcessor_unary>());
    processor.setNativeImpl("binary", std::make_unique<NodeProcessor_binary>());
    
    ConnectionData input;
    ConnectionData output;
    processor.processGraph(graph, input, output);

    CHECK(output.getValue("out") == "binary call node(callDataFrom(call1), callDataFrom(call2))");
}
