#include "nodes/openscad/Builtins.hpp"
#include "nodes/openscad/Builtins_helpers.hpp"

using namespace JNodes::openscad;
using namespace JNodes::core;

#define _OPENSCAD_NODE_CATEGORY Builtins::CATEGORY_FUNCTION.getName()

BINARY_NODE(function, concat, _OPENSCAD_NODE_CATEGORY, "Concatenate", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("concat(") + input.getValue("a", "0") + "," + input.getValue("b", "0") + std::string(")"));
}
BINARY_NODE(function, lookup, _OPENSCAD_NODE_CATEGORY, "Lookup", "value", DATA_VARIABLE, "table", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("lookup(") + input.getValue("value", "0") + ", " + input.getValue("table", "0") + std::string(")"));
}
UNARY_NODE(function, str, _OPENSCAD_NODE_CATEGORY, "To String", "x", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("str(") + input.getValue("x", "true") + std::string(")"));
}
UNARY_NODE(function, chr, _OPENSCAD_NODE_CATEGORY, "To Char", "x", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("chr(") + input.getValue("x", "true") + std::string(")"));
}
UNARY_NODE(function, ord, _OPENSCAD_NODE_CATEGORY, "To Ordinal", "x", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("ord(") + input.getValue("x", "true") + std::string(")"));
}
BINARY_NODE(function, search, _OPENSCAD_NODE_CATEGORY, "Search for value", "needle", DATA_VARIABLE, "haystack", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("search(") + input.getValue("needle", "0") + ", " + input.getValue("haystack", "[]") + std::string(")"));
}

////////////////////////////////////////
// Version
////////////////////////////////////////
void
Builtins::f_function_version_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    output.setValue("value", std::string("version()"));
}
////////////////////////////////////////
// Version Number
////////////////////////////////////////
void
Builtins::f_function_version_num_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    output.setValue("value", std::string("version_num()"));
}

////////////////////////////////////////
// Parent Module
////////////////////////////////////////
UNARY_NODE(function, parent_module, _OPENSCAD_NODE_CATEGORY, "Parent Module Name", "index", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("parent_module(") + input.getValue("index", "0") + std::string(")"));
}

