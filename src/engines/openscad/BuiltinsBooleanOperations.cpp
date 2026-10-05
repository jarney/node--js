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
// Union
////////////////////////////////////////
_OPENSCAD_PROCESSOR_DEF(op_union)
{
    std::string out;
    out += std::string("union() {\n");
    out +=     input.getValue("Geometry", "{}");
    out += std::string("}");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Difference
////////////////////////////////////////
_OPENSCAD_PROCESSOR_DEF(op_difference)
{
    std::string out;
    out += std::string("difference() {\n");
    out += std::string("    {\n");
    out +=     input.getValue("a", "{}");
    out += std::string("    }");
    out += std::string("    {\n");
    out +=     input.getValue("b", "{}");
    out += std::string("    }");
    out += std::string("}");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Intersection
////////////////////////////////////////
_OPENSCAD_PROCESSOR_DEF(op_intersection)
{
    std::string out;
    out += std::string("intersection() {\n");
    out +=     input.getValue("Geometry", "{}");
    out += std::string("}");
    output.setValue("Geometry", out);
}


