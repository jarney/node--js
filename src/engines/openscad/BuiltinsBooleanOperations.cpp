#include "node--js/engines/openscad/Builtins.hpp"

using namespace NodeJS::openscad;
using namespace NodeJS::core;

////////////////////////////////////////
// Union
////////////////////////////////////////
void
Builtins::f_op_union_process(const Node & node, const ConnectionData & input, ConnectionData & output)
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
void
Builtins::f_op_difference_process(const Node & node, const ConnectionData & input, ConnectionData & output)
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
void
Builtins::f_op_intersection_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    std::string out;
    out += std::string("intersection() {\n");
    out +=     input.getValue("Geometry", "{}");
    out += std::string("}");
    output.setValue("Geometry", out);
}


