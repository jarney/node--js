#include "node--js/engines/openscad/Builtins.hpp"

using namespace NodeJS::openscad;
using namespace NodeJS::core;

#define _OPENSCAD_PROCESSOR_DEF(name)                  \
    void Builtins::NodeProcessor_##name##_fn::process(      \
	const Node & node,                             \
	const ConnectionData & input,                  \
	ConnectionData & output                        \
	)

////////////////////////////////////////
// Circle
////////////////////////////////////////
_OPENSCAD_PROCESSOR_DEF(2d_circle)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "r");
    conditionalArg(args, input, node, "d");
    std::string out = std::string("circle(") + joinArguments(args) + std::string(")");
    output.setValue("Geometry", out);
    
}
////////////////////////////////////////
// Square
////////////////////////////////////////
_OPENSCAD_PROCESSOR_DEF(2d_square)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "size");
    conditionalArg(args, input, node, "center");
    std::string out = std::string("square(") + joinArguments(args) + std::string(")");
    output.setValue("Geometry", out);
    
}

////////////////////////////////////////
// Polygon
////////////////////////////////////////
_OPENSCAD_PROCESSOR_DEF(2d_polygon)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "points", "[]");
    conditionalArg(args, input, node, "paths");
    conditionalArg(args, input, node, "convexity");
    std::string out = std::string("polygon(") + joinArguments(args) + std::string(")");
    output.setValue("Geometry", out);
    
}

////////////////////////////////////////
// Text
////////////////////////////////////////
_OPENSCAD_PROCESSOR_DEF(2d_text)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "text");
    conditionalArg(args, input, node, "size");
    conditionalArg(args, input, node, "font");
    conditionalArg(args, input, node, "direction");
    conditionalArg(args, input, node, "language");
    conditionalArg(args, input, node, "script");
    conditionalArg(args, input, node, "halign");
    conditionalArg(args, input, node, "valign");
    conditionalArg(args, input, node, "spacing");
    conditionalArg(args, input, node, "em");
    std::string arguments = joinArguments(args);
    std::string out = std::string("text(") + arguments + std::string(")");

    output.setValue("Geometry", out);
    
}

////////////////////////////////////////
// Projection
////////////////////////////////////////
_OPENSCAD_PROCESSOR_DEF(2d_projection)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "cut");
    std::string out = std::string("projection(") + joinArguments(args) + std::string(")");
    output.setValue("Geometry", out);
    
}

