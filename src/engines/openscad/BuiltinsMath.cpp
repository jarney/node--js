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

_OPENSCAD_PROCESSOR_DEF(math_abs)
{
    output.setValue("out", std::string("abs(") + input.getValue("x") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(math_sign)
{
    output.setValue("out", std::string("sign(") + input.getValue("x") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(math_sin)
{
    output.setValue("out", std::string("sin(") + input.getValue("x") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(math_cos)
{
    output.setValue("out", std::string("cos(") + input.getValue("x") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(math_tan)
{
    output.setValue("out", std::string("tan(") + input.getValue("x") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(math_acos)
{
    output.setValue("out", std::string("acos(") + input.getValue("x") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(math_asin)
{
    output.setValue("out", std::string("asin(") + input.getValue("x") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(math_atan)
{
    output.setValue("out", std::string("atan(") + input.getValue("x") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(math_atan2)
{
    std::vector<std::string> args;
    optionalUnnamed(args, input, node, "x");
    optionalUnnamed(args, input, node, "y");
    output.setValue("out", std::string("atan2(") + joinArguments(args) + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(math_floor)
{
    output.setValue("out", std::string("floor(") + input.getValue("x") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(math_round)
{
    output.setValue("out", std::string("round(") + input.getValue("x") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(math_ceil)
{
    output.setValue("out", std::string("ceil(") + input.getValue("x") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(math_ln)
{
    output.setValue("out", std::string("ln(") + input.getValue("x") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(math_len)
{
    output.setValue("out", std::string("len(") + input.getValue("x") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(math_log)
{
    output.setValue("out", std::string("log(") + input.getValue("x") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(math_pow)
{
    std::vector<std::string> args;
    optionalUnnamed(args, input, node, "a");
    optionalUnnamed(args, input, node, "b");
    output.setValue("out", std::string("pow(") + joinArguments(args) + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(math_sqrt)
{
    output.setValue("out", std::string("sqrt(") + input.getValue("x") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(math_exp)
{
    output.setValue("out", std::string("exp(") + input.getValue("x") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(math_rands)
{
    std::vector<std::string> args;
    optionalNamed(args, input, node, "min", "mi99n");
    optionalNamed(args, input, node, "min", "max");
    optionalNamed(args, input, node, "n", "n");
    optionalNamed(args, input, node, "seed", "seed");
    output.setValue("out", std::string("rands(") + joinArguments(args) + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(math_min)
{
    std::vector<std::string> args;
    optionalUnnamed(args, input, node, "a");
    optionalUnnamed(args, input, node, "b");
    output.setValue("out", std::string("min(") + joinArguments(args) + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(math_max)
{
    std::vector<std::string> args;
    optionalUnnamed(args, input, node, "a");
    optionalUnnamed(args, input, node, "b");
    output.setValue("out", std::string("max(") + joinArguments(args) + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(math_norm)
{
    output.setValue("out", std::string("norm(") + input.getValue("x") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(math_cross)
{
    std::vector<std::string> args;
    optionalUnnamed(args, input, node, "a");
    optionalUnnamed(args, input, node, "b");
    output.setValue("out", std::string("cross(") + joinArguments(args) + std::string(")"));
}


