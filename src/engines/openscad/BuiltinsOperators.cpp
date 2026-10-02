#include "Builtins.hpp"

using namespace NodeJS::openscad;
using namespace NodeJS::core;

void
Builtins::f_operator_add_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + "+" + input.getValue("b", "0") + std::string(")"));
}
void
Builtins::f_operator_subtract_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + "-" + input.getValue("b", "0") + std::string(")"));
}
void
Builtins::f_operator_multiply_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + "*" + input.getValue("b", "0") + std::string(")"));
}
void
Builtins::f_operator_divide_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + "/" + input.getValue("b", "0") + std::string(")"));
}
void
Builtins::f_operator_modulo_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + "%" + input.getValue("b", "0") + std::string(")"));
}
void
Builtins::f_operator_exponentiate_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + "^" + input.getValue("b", "0") + std::string(")"));
}
void
Builtins::f_operator_lt_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + "<" + input.getValue("b", "0") + std::string(")"));
}
void
Builtins::f_operator_leq_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + "<=" + input.getValue("b", "0") + std::string(")"));
}
void
Builtins::f_operator_eq_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + "==" + input.getValue("b", "0") + std::string(")"));
}
void
Builtins::f_operator_neq_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + "!=" + input.getValue("b", "0") + std::string(")"));
}
void
Builtins::f_operator_geq_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + ">=" + input.getValue("b", "0") + std::string(")"));
}
void
Builtins::f_operator_gt_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + ">" + input.getValue("b", "0") + std::string(")"));
}
void
Builtins::f_operator_and_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + ")&&(" + input.getValue("b", "0") + std::string(")"));
}
void
Builtins::f_operator_binary_and_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + ")&(" + input.getValue("b", "0") + std::string(")"));
}
void
Builtins::f_operator_or_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + ")||(" + input.getValue("b", "0") + std::string(")"));
}
void
Builtins::f_operator_binary_or_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + ")|(" + input.getValue("b", "0") + std::string(")"));
}
void
Builtins::f_operator_binary_shl_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + ")<<(" + input.getValue("b", "0") + std::string(")"));
}
void
Builtins::f_operator_binary_shr_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + ")>>(" + input.getValue("b", "0") + std::string(")"));
}
void
Builtins::f_operator_not_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("!(") + input.getValue("a", "0") + std::string(")"));
}
void
Builtins::f_operator_negate_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("-(") + input.getValue("a", "0") + std::string(")"));
}
void
Builtins::f_operator_tilde_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("~(") + input.getValue("a", "0") + std::string(")"));
}
