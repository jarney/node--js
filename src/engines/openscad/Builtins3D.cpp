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
// Sphere
////////////////////////////////////////
_OPENSCAD_PROCESSOR_DEF(3d_sphere)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "r");
    conditionalArg(args, input, node, "d");
    std::string out = std::string("sphere(") + joinArguments(args) + std::string(")");
    output.setValue("Geometry", out);
    
}

////////////////////////////////////////
// Cube
////////////////////////////////////////
_OPENSCAD_PROCESSOR_DEF(3d_cube)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "size");
    conditionalArg(args, input, node, "center");
    std::string out = std::string("cube(") + joinArguments(args) + std::string(")");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Cylinder
////////////////////////////////////////
_OPENSCAD_PROCESSOR_DEF(3d_cylinder)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "h");
    conditionalArg(args, input, node, "r1");
    conditionalArg(args, input, node, "r2");
    conditionalArg(args, input, node, "center");
    conditionalArg(args, input, node, "r");
    conditionalArg(args, input, node, "d");
    conditionalArg(args, input, node, "d1");
    conditionalArg(args, input, node, "d2");
    std::string out = std::string("cylinder(") + joinArguments(args) + std::string(")");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Polyhedron
////////////////////////////////////////
_OPENSCAD_PROCESSOR_DEF(3d_polyhedron)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "points");
    conditionalArg(args, input, node, "faces");
    conditionalArg(args, input, node, "convexity");
    std::string out = std::string("polyhedron(") + joinArguments(args) + std::string(")");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Import
////////////////////////////////////////
_OPENSCAD_PROCESSOR_DEF(3d_import)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "file");
    conditionalArg(args, input, node, "center");
    conditionalArg(args, input, node, "convexity");
    conditionalArg(args, input, node, "id");
    conditionalArg(args, input, node, "layer");
    conditionalArg(args, input, node, "$fn");
    conditionalArg(args, input, node, "$fa");
    conditionalArg(args, input, node, "$fs");
    std::string out = std::string("import(") + joinArguments(args) + std::string(")");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Linear Extrude
////////////////////////////////////////
_OPENSCAD_PROCESSOR_DEF(3d_linear_extrude)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "height");
    conditionalArg(args, input, node, "v");
    conditionalArg(args, input, node, "scale");
    conditionalArg(args, input, node, "center");
    conditionalArg(args, input, node, "twist");
    conditionalArg(args, input, node, "slices");
    conditionalArg(args, input, node, "segments");
    conditionalArg(args, input, node, "convexity");
    conditionalArg(args, input, node, "h");
    conditionalArg(args, input, node, "$fn");
    std::string out = std::string("import(") + joinArguments(args) + std::string(")");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Rotate Extrude
////////////////////////////////////////
_OPENSCAD_PROCESSOR_DEF(3d_rotate_extrude)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "angle");
    conditionalArg(args, input, node, "start");
    conditionalArg(args, input, node, "convexity");
    conditionalArg(args, input, node, "a");
    std::string out = std::string("import(") + joinArguments(args) + std::string(")");
    output.setValue("Geometry", out);
}
////////////////////////////////////////
// Surface
////////////////////////////////////////
_OPENSCAD_PROCESSOR_DEF(3d_surface)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "file");
    conditionalArg(args, input, node, "center");
    conditionalArg(args, input, node, "invert");
    conditionalArg(args, input, node, "convexity");
    std::string out = std::string("surface(") + joinArguments(args) + std::string(")");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Import DXF Dimensions
////////////////////////////////////////
_OPENSCAD_PROCESSOR_DEF(3d_dxf_dim)
{
    std::vector<std::string> args;
    optionalNamed(args, input, node, "file", "file");
    optionalNamed(args, input, node, "name", "name");
    optionalNamed(args, input, node, "layer", "layer");
    optionalNamed(args, input, node, "origin", "origin");
    optionalNamed(args, input, node, "scale", "scale");
    std::string out = std::string("dxf_dim(") + joinArguments(args) + std::string(")");
    output.setValue("out", out);
}

////////////////////////////////////////
// Import DXF Cross (Origin)
////////////////////////////////////////
_OPENSCAD_PROCESSOR_DEF(3d_dxf_cross)
{
    std::vector<std::string> args;
    optionalNamed(args, input, node, "file", "file");
    optionalNamed(args, input, node, "layer", "layer");
    optionalNamed(args, input, node, "origin", "origin");
    optionalNamed(args, input, node, "scale", "scale");
    std::string out = std::string("dxf_cross(") + joinArguments(args) + std::string(")");
    output.setValue("out", out);
}

