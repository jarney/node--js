#include "nodes/openscad/Builtins.hpp"
#include "nodes/openscad/Builtins_helpers.hpp"
#include "NodeProgramSerializerOpenSCAD.hpp"

#include <QLabel>
#include <QCheckBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPlainTextEdit>

using namespace JNodes::openscad;
using namespace JNodes::core;

#define _OPENSCAD_NODE_CATEGORY Builtins::CATEGORY_SYNTAX.getName()

////////////////////////////////////////
// Assignment
////////////////////////////////////////
void
Builtins::f_syntax_assign_process(const Node & node, const NodePortData & input, NodePortData & output)
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
    if (node.hasValue("annotations.Description")) {
	out += std::string("//") + node.getValue("annotations.Description");
	out += std::string("\n");
    }
    out += node.getValue("variable_name", "x") + std::string("= ") + value + std::string(";");
    out += std::string("\n");
    output.setValue("out", out);
}

void
Builtins::f_syntax_assign_initializer(Node & node)
{
    if (!node.hasValue("variable_name")) {
	node.setValue("variable_name", "x");
    }
}

////////////////////////////////////////
// Assign List
////////////////////////////////////////
void
Builtins::f_syntax_assign_list_process(const Node & node, const NodePortData & input, NodePortData & output)
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
void
Builtins::f_syntax_variable_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::string out = node.getValue("variable_name");
    output.setValue("variable", out);
}

void
Builtins::f_syntax_variable_initializer(Node & node)
{
    if (!node.hasValue("variable_name")) {
	node.setValue("variable_name", "x");
    }
}
////////////////////////////////////////
// Define Module
////////////////////////////////////////
void
Builtins::f_syntax_module_process(const Node & node, const NodePortData & input, NodePortData & output)
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
void
Builtins::f_syntax_function_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::string graph = node.getValue("name", "undefined");
    
    NodeGraph *bodyGraph = node.getGraph().getParent().getGraph(graph);
    if (!bodyGraph) {
	fprintf(stderr, "Body graph for %s does not exist\n", graph.c_str());
	return;
    }
    std::string bodyString = NodeProgramSerializerOpenSCAD::toString(*bodyGraph);
    
    std::string out = std::string("function ") + graph + std::string("()") + bodyString;
    output.setValue("Geometry", out);

    // Get the graph for the body and process it to generate the output for it.
    // The output of the sub-graph is the function's body.

}

////////////////////////////////////////
// Include
////////////////////////////////////////
void
Builtins::f_syntax_include_process(const Node & node, const NodePortData & input, NodePortData & output)
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
void
Builtins::f_syntax_use_process(const Node & node, const NodePortData & input, NodePortData & output)
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
void
Builtins::syntax_custom_node_processor(const JNodes::core::Node & node, const JNodes::core::NodePortData & input, JNodes::core::NodePortData & output)
{
    std::vector<std::string> args;
    // If the output is 'value' then
    // it is a custom function.
    if (node.hasOutputPort("value")) {
	std::string out;
	for (int i = 0; i < node.nPorts(QtNodes::PortType::In); i++) {
	    std::string argname = node.getInputPortName(i);
	    conditionalArg(args, input, node, argname);
	}

	out += node.name().toStdString();
	out += std::string("(") + joinArguments(args) + std::string(")");
	output.setValue("value", out);
    }
    // Otherwise, it is a custom module.
    else {
	output.setValue("Geometry", "This is the output of a custom module");
    }
}
