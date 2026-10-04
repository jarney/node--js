#include "node--js/engines/openscad/Builtins.hpp"

using namespace NodeJS::openscad;
using namespace NodeJS::core;

void
Builtins::f_typetest_is_bool_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("is_bool(") + input.getValue("x", "true") + std::string(")"));
}
void
Builtins::f_typetest_is_string_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("is_string(") + input.getValue("x", "true") + std::string(")"));
}
void
Builtins::f_typetest_is_num_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("is_num(") + input.getValue("x", "true") + std::string(")"));
}
void
Builtins::f_typetest_is_function_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("is_function(") + input.getValue("x", "true") + std::string(")"));
}
void
Builtins::f_typetest_is_list_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("is_list(") + input.getValue("x", "true") + std::string(")"));
}
void
Builtins::f_typetest_is_undef_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("is_undef(") + input.getValue("x", "true") + std::string(")"));
}
