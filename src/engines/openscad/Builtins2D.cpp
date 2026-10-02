#include "nodes/openscad/Builtins.hpp"
#include "nodes/openscad/Builtins_helpers.hpp"

using namespace JNodes::openscad;
using namespace JNodes::core;

#define _OPENSCAD_NODE_CATEGORY Builtins::CATEGORY_2D.getName()

////////////////////////////////////////
// Circle
////////////////////////////////////////
void
Builtins::f_2d_circle_process(const Node & node, const NodePortData & input, NodePortData & output)
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
void
Builtins::f_2d_square_process(const Node & node, const NodePortData & input, NodePortData & output)
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
void
Builtins::f_2d_polygon_process(const Node & node, const NodePortData & input, NodePortData & output)
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
void
Builtins::f_2d_text_process(const Node & node, const NodePortData & input, NodePortData & output)
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
void
Builtins::f_2d_projection_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "cut");
    std::string out = std::string("projection(") + joinArguments(args) + std::string(")");
    output.setValue("Geometry", out);
    
}

