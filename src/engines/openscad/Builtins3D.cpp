#include "nodes/openscad/Builtins.hpp"
#include "nodes/openscad/Builtins_helpers.hpp"

using namespace JNodes::openscad;
using namespace JNodes::core;

#define _OPENSCAD_NODE_CATEGORY Builtins::CATEGORY_3D.getName()

////////////////////////////////////////
// Sphere
////////////////////////////////////////
void
Builtins::f_3d_sphere_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "r");
    conditionalArg(args, input, node, "d");
    std::string out = std::string("sphere(") + joinArguments(args) + std::string(");");
    output.setValue("Geometry", out);
    
}

////////////////////////////////////////
// Cube
////////////////////////////////////////
void
Builtins::f_3d_cube_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "size");
    conditionalArg(args, input, node, "center");
    std::string out = std::string("cube(") + joinArguments(args) + std::string(");");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Cylinder
////////////////////////////////////////
void
Builtins::f_3d_cylinder_process(const Node & node, const NodePortData & input, NodePortData & output)
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
    std::string out = std::string("cylinder(") + joinArguments(args) + std::string(");");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Polyhedron
////////////////////////////////////////
void
Builtins::f_3d_polyhedron_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "points");
    conditionalArg(args, input, node, "faces");
    conditionalArg(args, input, node, "convexity");
    std::string out = std::string("polyhedron(") + joinArguments(args) + std::string(");");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Import
////////////////////////////////////////
void
Builtins::f_3d_import_process(const Node & node, const NodePortData & input, NodePortData & output)
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
    std::string out = std::string("import(") + joinArguments(args) + std::string(");");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Linear Extrude
////////////////////////////////////////
void
Builtins::f_3d_linear_extrude_process(const Node & node, const NodePortData & input, NodePortData & output)
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
    std::string out = std::string("import(") + joinArguments(args) + std::string(");");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Rotate Extrude
////////////////////////////////////////
void
Builtins::f_3d_rotate_extrude_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "angle");
    conditionalArg(args, input, node, "start");
    conditionalArg(args, input, node, "convexity");
    conditionalArg(args, input, node, "a");
    std::string out = std::string("import(") + joinArguments(args) + std::string(");");
    output.setValue("Geometry", out);
}
////////////////////////////////////////
// Surface
////////////////////////////////////////
void
Builtins::f_3d_surface_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "file");
    conditionalArg(args, input, node, "center");
    conditionalArg(args, input, node, "invert");
    conditionalArg(args, input, node, "convexity");
    std::string out = std::string("surface(") + joinArguments(args) + std::string(");");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Import DXF Dimensions
////////////////////////////////////////
void
Builtins::f_3d_dxf_dim_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "file");
    conditionalArg(args, input, node, "name");
    conditionalArg(args, input, node, "layer");
    conditionalArg(args, input, node, "origin");
    conditionalArg(args, input, node, "scale");
    std::string out = std::string("dxf_dim(") + joinArguments(args) + std::string(");");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Import DXF Cross (Origin)
////////////////////////////////////////
void
Builtins::f_3d_dxf_cross_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "file");
    conditionalArg(args, input, node, "layer");
    conditionalArg(args, input, node, "origin");
    conditionalArg(args, input, node, "scale");
    std::string out = std::string("dxf_cross(") + joinArguments(args) + std::string(");");
    output.setValue("Geometry", out);
}

