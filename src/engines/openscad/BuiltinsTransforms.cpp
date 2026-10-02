#include "Builtins.hpp"

using namespace NodeJS::openscad;
using namespace NodeJS::core;

////////////////////////////////////////
// Translate
////////////////////////////////////////
void
Builtins::f_xform_translate_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "v");
    
    std::string out;
    out += std::string("translate(") + joinArguments(args) + std::string(") {\n");
    out += input.getValue("Geometry", "");
    out += std::string("}\n");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Rotate
////////////////////////////////////////
void
Builtins::f_xform_rotate_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "a");
    conditionalArg(args, input, node, "v");
    
    std::string out;
    out += std::string("rotate(") + joinArguments(args) + std::string(") {\n");
    out += input.getValue("Geometry", "");
    out += std::string("}\n");
    output.setValue("Geometry", out);
}
////////////////////////////////////////
// Scale
////////////////////////////////////////
void
Builtins::f_xform_scale_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "v");

    std::string out = std::string();
    out += std::string("scale(") + joinArguments(args) + std::string(") {\n");
    out += input.getValue("Geometry", "");
    out += std::string("}\n");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Resize
////////////////////////////////////////
void
Builtins::f_xform_resize_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    std::string out = std::string();
    std::vector<std::string> args;
    conditionalArg(args, input, node, "newsize");
    conditionalArg(args, input, node, "auto");
    conditionalArg(args, input, node, "convexity");

    out += std::string("resize(") + joinArguments(args) + std::string(") {\n");
    out += input.getValue("Geometry", "");
    out += std::string("}\n");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Mirror
////////////////////////////////////////
void
Builtins::f_xform_mirror_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "v");
    
    std::string out;
    out += std::string("mirror(") + joinArguments(args) + std::string(") {\n");
    out += input.getValue("Geometry", "");
    out += std::string("}\n");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Multmatrix
////////////////////////////////////////
void
Builtins::f_xform_multmatrix_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "m");
    
    std::string out;
    out += std::string("multmatrix(") + joinArguments(args) + std::string(") {\n");
    out += input.getValue("Geometry", "");
    out += std::string("}\n");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Color
////////////////////////////////////////
void
Builtins::f_xform_color_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "convexity");
    
    std::string out = std::string();
    out += std::string("color(") + joinArguments(args) + std::string(") {\n");
    out += input.getValue("Geometry", "");
    out += std::string("}\n");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Offset
////////////////////////////////////////
void
Builtins::f_xform_offset_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "r");
    conditionalArg(args, input, node, "delta");
    conditionalArg(args, input, node, "chamfer");
    
    std::string out = std::string();
    out += std::string("offset(") + joinArguments(args) + std::string(") {\n");
    out += input.getValue("Geometry", "");
    out += std::string("}\n");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Convex Hull
////////////////////////////////////////
void
Builtins::f_xform_hull_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    std::string out;
    out += std::string("hull() {\n");
    out += std::string("    {\n");
    out += input.getValue("a", "{}");
    out += std::string("    }");
    out += std::string("})");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Fill
////////////////////////////////////////
void
Builtins::f_xform_fill_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("Geometry", std::string("fill() {\n") +
	std::string("    {\n") + 
        input.getValue("a", "{}") +
	std::string("    }") + 
        std::string("})"));
}

////////////////////////////////////////
// Minkowski
////////////////////////////////////////
void
Builtins::f_xform_minkowski_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "convexity");
    
    output.setValue("Geometry", std::string("minkowski(") + joinArguments(args) + std::string(") {\n") +
	std::string("    {\n") + 
        input.getValue("a", "{}") +
	std::string("    }") + 
	std::string("    {\n") +
        input.getValue("b", "{}") +
	std::string("    }") + 
        std::string("})"));
}

