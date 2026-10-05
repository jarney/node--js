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
// List Index
////////////////////////////////////////
_OPENSCAD_PROCESSOR_DEF(list_index)
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
_OPENSCAD_PROCESSOR_DEF(list_get_xyz)
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
_OPENSCAD_PROCESSOR_DEF(list_set_xyz)
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
_OPENSCAD_PROCESSOR_DEF(list_get_xy)
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
_OPENSCAD_PROCESSOR_DEF(list_set_xy)
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
_OPENSCAD_PROCESSOR_DEF(list_get_rgba)
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
_OPENSCAD_PROCESSOR_DEF(list_set_rgba)
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
_OPENSCAD_PROCESSOR_DEF(list_set_range)
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
