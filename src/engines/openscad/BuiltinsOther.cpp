#include "node--js/engines/openscad/Builtins.hpp"

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
// Echo
////////////////////////////////////////
_OPENSCAD_PROCESSOR_DEF(other_echo)
{
    output.setValue("out", std::string("echo(") + input.getValue("value", "1") + std::string(")"));
}
////////////////////////////////////////
// Render
////////////////////////////////////////
_OPENSCAD_PROCESSOR_DEF(other_render)
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
_OPENSCAD_PROCESSOR_DEF(other_children)
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
_OPENSCAD_PROCESSOR_DEF(other_assert)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "condition");
    
    std::string out;
    out += std::string("assert(") + joinArguments(args) + std::string(")");
    output.setValue("Geometry", out);
}
