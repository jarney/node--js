#include "node--js/engines/openscad/Builtins.hpp"

using namespace NodeJS::openscad;
using namespace NodeJS::core;

////////////////////////////////////////
// For
////////////////////////////////////////
void
Builtins::f_flow_for_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    std::string out = std::string();
    out += std::string("for (");
    out += std::string(node.getData().getValue("variable"));
    out += std::string(" = ");
    if (input.hasValue("range")) {
	out += input.getValue("range");
    }
    else {
	out += std::string("[");
	out += input.getValue("start", "0");
	out += std::string(":");
	out += input.getValue("end", "0");
	if (input.hasValue("increment")) {
	    out += std::string(":");
	    out += input.getValue("increment");
	}
	out += std::string("]");
    }
    out += std::string(") {");

    std::string bodyGraphId = node.getData().getValue("graph");

    // This is a big TODO once we re-do the
    // processing engine for the new structure.
#if 0
    const NodeGraph *subgraph =
	node.getGraph().getParent().getGraph(bodyGraphId);
    out += NodeProgramSerializerOpenSCAD::toString(*subgraph);
#endif
    
    out += std::string("}");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Intersection For
////////////////////////////////////////
void
Builtins::f_flow_intersection_for_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    // TODO: Call/evaluate sub-flow
    std::string out = std::string();
    out += std::string("intersection_for(") + std::string("var = ") + input.getValue("var", "[1:10]") + std::string(") {\n");
    out += input.getValue("Geometry", "");
    out += std::string("}\n");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// If
////////////////////////////////////////
void
Builtins::f_flow_if_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    std::string out = std::string();
    out += std::string("if(");
    out += input.getValue("condition", "true");
    out += std::string(") {\n");
    out += input.getValue("a", "");
    out += std::string("} else {\n");
    out += input.getValue("b", "");
    out += std::string("}\n");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Let
////////////////////////////////////////
void
Builtins::f_flow_let_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    // TODO: Call/evaluate sub-flow
    std::string out = std::string();
    out += std::string("let(") + std::string("var = ") + input.getValue("var", "[1:10]") + std::string(") {\n");
    out += input.getValue("Geometry", "");
    out += std::string("}\n");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Comment
////////////////////////////////////////

////////////////////////////////////////
// Group
////////////////////////////////////////
void
Builtins::f_flow_group_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("Geometry", std::string("group() {\n") +
        input.getValue("a", "{}") +
        std::string("});"));
}

////////////////////////////////////////
// Output
////////////////////////////////////////
void
Builtins::f_flow_module_output_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    std::string s = input.getValue("out", "//No Geometry Output\n");
    output.setValue("out", s);
}

////////////////////////////////////////
// Output
////////////////////////////////////////
void
Builtins::f_flow_function_output_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    std::string s = input.getValue("out", "//No Geometry Output\n");
    output.setValue("out", s);
}



