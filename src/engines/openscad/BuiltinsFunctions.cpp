#include "Builtins.hpp"

using namespace NodeJS::openscad;
using namespace NodeJS::core;

void
Builtins::f_function_concat_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("concat(") + input.getValue("a", "0") + "," + input.getValue("b", "0") + std::string(")"));
}
void
Builtins::f_function_lookup_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("lookup(") + input.getValue("value", "0") + ", " + input.getValue("table", "0") + std::string(")"));
}
void
Builtins::f_function_str_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("str(") + input.getValue("x", "true") + std::string(")"));
}
void
Builtins::f_function_chr_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("chr(") + input.getValue("x", "true") + std::string(")"));
}
void
Builtins::f_function_ord_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("ord(") + input.getValue("x", "true") + std::string(")"));
}
void
Builtins::f_function_search_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("search(") + input.getValue("needle", "0") + ", " + input.getValue("haystack", "[]") + std::string(")"));
}

////////////////////////////////////////
// Version
////////////////////////////////////////
void
Builtins::f_function_version_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("value", std::string("version()"));
}
////////////////////////////////////////
// Version Number
////////////////////////////////////////
void
Builtins::f_function_version_num_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("value", std::string("version_num()"));
}

////////////////////////////////////////
// Parent Module
////////////////////////////////////////
void
Builtins::f_function_parent_module_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("parent_module(") + input.getValue("index", "0") + std::string(")"));
}

