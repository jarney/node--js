#include "nodes/openscad/Builtins.hpp"
#include "nodes/openscad/Builtins_helpers.hpp"

using namespace JNodes::openscad;
using namespace JNodes::core;

#define _OPENSCAD_NODE_CATEGORY Builtins::CATEGORY_LIST.getName()

////////////////////////////////////////
// List Index
////////////////////////////////////////
void
Builtins::f_list_index_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::vector<std::string> args;
    std::string out;
    out += input.getValue("list", "[]");
    out += std::string("[") +  input.getValue("index", "0") + std::string("]");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// List get xyz
////////////////////////////////////////
void
Builtins::f_list_get_xyz_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::vector<std::string> args;
    std::string x = input.getValue("list", "[0,0,0]") + std::string("[0]");
    std::string y = input.getValue("list", "[0,0,0]") + std::string("[1]");
    std::string z = input.getValue("list", "[0,0,0]") + std::string("[2]");
    output.setValue("x", x);
    output.setValue("y", y);
    output.setValue("z", z);
}

////////////////////////////////////////
// List set xyz
////////////////////////////////////////
void
Builtins::f_list_set_xyz_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::string out;
    out += std::string("[");
    out += input.getValue("x", "undef") + std::string(",");
    out += input.getValue("y", "undef") + std::string(",");
    out += input.getValue("z", "undef");
    out += std::string("]");
    output.setValue("list", out);
}

////////////////////////////////////////
// List get xyz
////////////////////////////////////////
void
Builtins::f_list_get_xy_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::vector<std::string> args;
    std::string x = input.getValue("list", "[0,0]") + std::string("[0]");
    std::string y = input.getValue("list", "[0,0]") + std::string("[1]");
    output.setValue("x", x);
    output.setValue("y", y);
}

////////////////////////////////////////
// List set xy
////////////////////////////////////////
void
Builtins::f_list_set_xy_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::string out;
    out += std::string("[");
    out += input.getValue("x", "undef") + std::string(",");
    out += input.getValue("y", "undef");
    out += std::string("]");
    output.setValue("list", out);
}

////////////////////////////////////////
// List get xyz
////////////////////////////////////////
void
Builtins::f_list_get_rgba_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::vector<std::string> args;
    std::string r = input.getValue("list", "[1,1,1,0]") + std::string("[0]");
    std::string g = input.getValue("list", "[1,1,1,0]") + std::string("[1]");
    std::string b = input.getValue("list", "[1,1,1,0]") + std::string("[2]");
    std::string a = input.getValue("list", "[1,1,1,0]") + std::string("[3]");
    output.setValue("r", r);
    output.setValue("g", g);
    output.setValue("b", b);
    output.setValue("a", a);
}

////////////////////////////////////////
// List set rgba
////////////////////////////////////////
void
Builtins::f_list_set_rgba_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::string out;
    out += std::string("[");
    out += input.getValue("r", "1") + std::string(",");
    out += input.getValue("g", "1") + std::string(",");
    out += input.getValue("b", "1") + std::string(",");
    out += input.getValue("a", "0");
    out += std::string("]");
    output.setValue("list", out);
    
}

////////////////////////////////////////
// List set range
////////////////////////////////////////
void
Builtins::f_list_set_range_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::string out;
    out += std::string("[");
    out += input.getValue("start", "0");
    out += std::string(":") + input.getValue("end", "1");
    if (input.hasValue("increment")) {
	out += std::string(":") + input.getValue("increment", "1");
    }
    out += std::string("]");
    output.setValue("range", out);
}
