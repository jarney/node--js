#include "node--js/engines/openscad/Builtins.hpp"

using namespace NodeJS::openscad;
using namespace NodeJS::core;

#define _OPENSCAD_PROCESSOR_DEF(name)                  \
    void Builtins::NodeProcessor_##name##_fn::process(      \
	const Node & node,                             \
	const ConnectionData & input,                  \
	ConnectionData & output                        \
	)

_OPENSCAD_PROCESSOR_DEF(operator_add)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + "+" + input.getValue("b", "0") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(operator_subtract)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + "-" + input.getValue("b", "0") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(operator_multiply)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + "*" + input.getValue("b", "0") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(operator_divide)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + "/" + input.getValue("b", "0") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(operator_modulo)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + "%" + input.getValue("b", "0") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(operator_exponentiate)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + "^" + input.getValue("b", "0") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(operator_lt)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + "<" + input.getValue("b", "0") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(operator_leq)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + "<=" + input.getValue("b", "0") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(operator_eq)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + "==" + input.getValue("b", "0") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(operator_neq)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + "!=" + input.getValue("b", "0") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(operator_geq)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + ">=" + input.getValue("b", "0") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(operator_gt)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + ">" + input.getValue("b", "0") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(operator_and)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + ")&&(" + input.getValue("b", "0") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(operator_binary_and)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + ")&(" + input.getValue("b", "0") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(operator_or)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + ")||(" + input.getValue("b", "0") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(operator_binary_or)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + ")|(" + input.getValue("b", "0") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(operator_binary_shl)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + ")<<(" + input.getValue("b", "0") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(operator_binary_shr)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + ")>>(" + input.getValue("b", "0") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(operator_not)
{
    output.setValue("out", std::string("!(") + input.getValue("a", "0") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(operator_negate)
{
    output.setValue("out", std::string("-(") + input.getValue("a", "0") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(operator_tilde)
{
    output.setValue("out", std::string("~(") + input.getValue("a", "0") + std::string(")"));
}
