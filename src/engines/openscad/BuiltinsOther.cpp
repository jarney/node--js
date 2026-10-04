#include "node--js/engines/openscad/Builtins.hpp"

using namespace NodeJS::openscad;
using namespace NodeJS::core;

////////////////////////////////////////
// Echo
////////////////////////////////////////
void
Builtins::f_other_echo_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("echo(") + input.getValue("value", "1") + std::string(")"));
}
////////////////////////////////////////
// Render
////////////////////////////////////////
void
Builtins::f_other_render_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "convexity");
    std::string out;
    out += std::string("render(") + joinArguments(args) + std::string(") {\n");
    out +=     input.getValue("Geometry", "{}");
    out += "}";
    output.setValue("Geometry", out);
    
}
////////////////////////////////////////
// Children
////////////////////////////////////////
void
Builtins::f_other_children_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "index");
    
    std::string out;
    out += std::string("children(") + joinArguments(args) + std::string(")");
    output.setValue("Geometry", out);
    
}

////////////////////////////////////////
// Assert
////////////////////////////////////////
void
Builtins::f_other_assert_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "condition");
    
    std::string out;
    out += std::string("assert(") + joinArguments(args) + std::string(")");
    output.setValue("Geometry", out);
}
