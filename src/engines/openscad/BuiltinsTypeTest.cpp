#include "nodes/openscad/Builtins.hpp"
#include "nodes/openscad/Builtins_helpers.hpp"

using namespace JNodes::openscad;
using namespace JNodes::core;

#define _OPENSCAD_NODE_CATEGORY Builtins::CATEGORY_TYPETEST.getName()

UNARY_NODE(typetest, is_bool, _OPENSCAD_NODE_CATEGORY, "Is Boolean", "x", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("is_bool(") + input.getValue("x", "true") + std::string(")"));
}
UNARY_NODE(typetest, is_string, _OPENSCAD_NODE_CATEGORY, "Is String", "x", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("is_string(") + input.getValue("x", "true") + std::string(")"));
}
UNARY_NODE(typetest, is_num, _OPENSCAD_NODE_CATEGORY, "Is Number", "x", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("is_num(") + input.getValue("x", "true") + std::string(")"));
}
UNARY_NODE(typetest, is_function, _OPENSCAD_NODE_CATEGORY, "Is Function", "x", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("is_function(") + input.getValue("x", "true") + std::string(")"));
}
UNARY_NODE(typetest, is_list, _OPENSCAD_NODE_CATEGORY, "Is List", "x", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("is_list(") + input.getValue("x", "true") + std::string(")"));
}
UNARY_NODE(typetest, is_undef, _OPENSCAD_NODE_CATEGORY, "Is Undefined", "x", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("is_undef(") + input.getValue("x", "true") + std::string(")"));
}
