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
// Translate
////////////////////////////////////////
_OPENSCAD_PROCESSOR_DEF(xform_translate)
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
_OPENSCAD_PROCESSOR_DEF(xform_rotate)
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
_OPENSCAD_PROCESSOR_DEF(xform_scale)
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
_OPENSCAD_PROCESSOR_DEF(xform_resize)
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
_OPENSCAD_PROCESSOR_DEF(xform_mirror)
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
_OPENSCAD_PROCESSOR_DEF(xform_multmatrix)
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
_OPENSCAD_PROCESSOR_DEF(xform_color)
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
_OPENSCAD_PROCESSOR_DEF(xform_offset)
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
_OPENSCAD_PROCESSOR_DEF(xform_hull)
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
_OPENSCAD_PROCESSOR_DEF(xform_fill)
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
_OPENSCAD_PROCESSOR_DEF(xform_minkowski)
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

