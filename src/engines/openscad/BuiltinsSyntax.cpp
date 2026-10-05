#include "node--js/engines/openscad/Builtins.hpp"
#include "node--js/NodeGraph.hpp"
#include "node--js/NodeModule.hpp"

using namespace NodeJS::openscad;
using namespace NodeJS::core;

#define _OPENSCAD_PROCESSOR_DEF(name)                  \
    void Builtins::NodeProcessor_##name##_fn::process( \
	Processor & processor,                         \
	const Node & node,                             \
	const ConnectionData & input,                  \
	ConnectionData & output                        \
	)

////////////////////////////////////////
// Assignment
////////////////////////////////////////
_OPENSCAD_PROCESSOR_DEF(syntax_assign)
{
    std::string value;
    if (input.hasValue("value")) {
	value = input.getValue("value");
    }
    else {
	value = "undef";
    }
    
    std::string out;

    // XXX TODO:
    // The format of annotations is a bit weird
    // in the comment parser, so we're not really
    // handling all cases yet.  Also, if there is
    // any newline inside the annotation, that's bad news.
    if (node.getData().hasValue("annotations.Description")) {
	out += std::string("//") + node.getData().getValue("annotations.Description");
	out += std::string("\n");
    }
    out += node.getData().getValue("variable_name", "x") + std::string("= ") + value + std::string(";");
    out += std::string("\n");
    output.setValue("assignment", out);
}

////////////////////////////////////////
// Assign List
////////////////////////////////////////
_OPENSCAD_PROCESSOR_DEF(syntax_assign_list)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "r");
    conditionalArg(args, input, node, "d");
    std::string out = std::string("sphere(") + joinArguments(args) + std::string(")");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Variable
////////////////////////////////////////
_OPENSCAD_PROCESSOR_DEF(syntax_variable)
{
    std::string out = node.getData().getValue("variable_name");
    output.setValue("variable", out);
}

////////////////////////////////////////
// Define Module
////////////////////////////////////////
_OPENSCAD_PROCESSOR_DEF(syntax_module)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "r");
    conditionalArg(args, input, node, "d");
    std::string out = std::string("sphere(") + joinArguments(args) + std::string(")");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Define Function
////////////////////////////////////////
_OPENSCAD_PROCESSOR_DEF(syntax_function)
{
    std::string graph = node.getData().getValue("graph", "undefined");
    
    NodeGraph *bodyGraph = node.getGraph().getModule().getGraph(graph);
    if (!bodyGraph) {
	fprintf(stderr, "Body graph for %s does not exist\n", graph.c_str());
	return;
    }

    ConnectionData bodyInput;
    ConnectionData bodyOutput;
    processor.processGraph(*bodyGraph, bodyInput, bodyOutput);

    std::string out = std::string("function ") + graph + std::string("()") + bodyOutput.getValue("out");
    output.setValue("out", out);

    // Get the graph for the body and process it to generate the output for it.
    // The output of the sub-graph is the function's body.

}

////////////////////////////////////////
// Include
////////////////////////////////////////
_OPENSCAD_PROCESSOR_DEF(syntax_include)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "r");
    conditionalArg(args, input, node, "d");
    std::string out = std::string("sphere(") + joinArguments(args) + std::string(")");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Use
////////////////////////////////////////
_OPENSCAD_PROCESSOR_DEF(syntax_use)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "r");
    conditionalArg(args, input, node, "d");
    std::string out = std::string("sphere(") + joinArguments(args) + std::string(")");
    output.setValue("Geometry", out);
}
////////////////////////////////////////
// This is how custom nodes like modules
// and functions get processed.
////////////////////////////////////////
_OPENSCAD_PROCESSOR_DEF(syntax_custom_node)
{
    std::vector<std::string> args;
    // If the output is 'value' then
    // it is a custom function.
    const NodeType & nodeType = node.getType();
    if (nodeType.hasOutputPort("value")) {
	std::string out;
	for (int i = 0; i < nodeType.getInputPortCount(); i++) {
	    std::string argname = nodeType.getInputPortName(i);
	    conditionalArg(args, input, node, argname);
	}

	out += nodeType.getId();
	out += std::string("(") + joinArguments(args) + std::string(")");
	output.setValue("value", out);
    }
    // Otherwise, it is a custom module.
    else {
	output.setValue("Geometry", "This is the output of a custom module");
    }
}
