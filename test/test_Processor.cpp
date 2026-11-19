#include <catch2/catch_all.hpp>

#include "node--js/ConnectionData.hpp"
#include "node--js/NodeModule.hpp"
#include "node--js/NodeGraph.hpp"
#include "node--js/NodeType.hpp"
#include "node--js/Natives.hpp"
#include "node--js/Processor.hpp"
#include "node--js/ModuleLoader.hpp"

using namespace NodeJS::core;

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

TEST_CASE("test_Processor_minimal", "[NodeJS][core][Processor]")
{
    ModuleLoader loader;
    auto nodeModule = loader.newModule("anonymous");
    NodeGraph & graph = *nodeModule->addGraph("graph");
    ConnectionData nodeData;
    NodeType nodeType;

    NodeType unaryType;
    unaryType.setId("unary", "testing");
    unaryType.setType(NodeType::Type::NATIVE);

    NodeType binType;
    binType.setId("binary", "testing");
    binType.setType(NodeType::Type::NATIVE);

    NodeType inputType;
    inputType.setId("input", "org.ensor.nodejs.core");
    inputType.setType(NodeType::Type::NATIVE);
    
    NodeType outputType;
    outputType.setId("output", "org.ensor.nodejs.core");
    outputType.setType(NodeType::Type::NATIVE);
    
    const Node & input = graph.newNode(inputType, "input", nodeData);
    const Node & output = graph.newNode(outputType, "output", nodeData);
    const Node & main = graph.newNode(binType, "main", nodeData);
    const Node & call1 = graph.newNode(unaryType, "call1", nodeData);
    const Node & call2 = graph.newNode(unaryType, "call2", nodeData);

    graph.newEdge(call1.getId(), "out", main.getId(), "in1");
    graph.newEdge(call2.getId(), "out", main.getId(), "in2");
    graph.newEdge(main.getId(), "out", output.getId(), "out");

    Processor processor;
    processor.setNativeImpl(NodeProcessor_input::ID, std::make_unique<NodeProcessor_input>());
    processor.setNativeImpl(NodeProcessor_output::ID, std::make_unique<NodeProcessor_output>());
    processor.setNativeImpl("testing.unary", std::make_unique<NodeProcessor_unary>());
    processor.setNativeImpl("testing.binary", std::make_unique<NodeProcessor_binary>());
    
    ConnectionData inputData;
    ConnectionData outputData;
    processor.processGraph(graph, inputData, outputData);

    CHECK(outputData.getValue("out") == "binary call node(callDataFrom(call1), callDataFrom(call2))");
}

