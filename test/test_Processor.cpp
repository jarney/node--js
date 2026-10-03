#include <catch2/catch_all.hpp>

#include "node--js/ConnectionData.hpp"
#include "node--js/NodeModule.hpp"
#include "node--js/NodeGraph.hpp"
#include "node--js/NodeType.hpp"
#include "node--js/Processor.hpp"

using namespace NodeJS::core;

static void
binary_processor(const Node & node, const ConnectionData & fromData, ConnectionData & toData)
{
    toData.setValue("out", "binary call node(" + fromData.getValue("in1") + ", " + fromData.getValue("in2") + ")");
}

static void
unary_processor(const Node & node, const ConnectionData & fromData, ConnectionData & toData)
{
    toData.setValue("out", "callDataFrom(" + node.getId() + ")");
}

TEST_CASE("Processor minimal", "[NodeJS][core][Processor]")
{
    NodeModule module;
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

    graph.newEdge(main.getId(), "in1", call1.getId(), "out");
    graph.newEdge(main.getId(), "in2", call2.getId(), "out");

    Processor processor;
    processor.setNativeImpl("unary", unary_processor);
    processor.setNativeImpl("binary", binary_processor);
    
    //out =
    processor.processGraph(graph);
    fprintf(stderr, "We really processed a graph?\n");

}
