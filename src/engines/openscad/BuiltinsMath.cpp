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
    output.setValue("out", std::string("abs(") + input.getValue("x", "0") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(math_sign)
{
    output.setValue("out", std::string("sign(") + input.getValue("x", "0") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(math_sin)
{
    output.setValue("out", std::string("sin(") + input.getValue("x", "0") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(math_cos)
{
    output.setValue("out", std::string("cos(") + input.getValue("x", "0") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(math_tan)
{
    output.setValue("out", std::string("tan(") + input.getValue("x", "0") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(math_acos)
{
    output.setValue("out", std::string("acos(") + input.getValue("x", "1") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(math_asin)
{
    output.setValue("out", std::string("asin(") + input.getValue("x", "0") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(math_atan)
{
    output.setValue("out", std::string("atan(") + input.getValue("x", "0") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(math_atan2)
{
    output.setValue("out", std::string("atan2(") + input.getValue("x", "0") + "," + input.getValue("y", "0") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(math_floor)
{
    output.setValue("out", std::string("floor(") + input.getValue("x", "0") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(math_round)
{
    output.setValue("out", std::string("round(") + input.getValue("x", "0") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(math_ceil)
{
    output.setValue("out", std::string("ceil(") + input.getValue("x", "0") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(math_ln)
{
    output.setValue("out", std::string("ln(") + input.getValue("x", "2.718") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(math_len)
{
    output.setValue("out", std::string("len(") + input.getValue("x", "0") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(math_log)
{
    output.setValue("out", std::string("log(") + input.getValue("x", "10") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(math_pow)
{
    output.setValue("out", std::string("pow(") + input.getValue("a", "0") + "," + input.getValue("b", "0") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(math_sqrt)
{
    output.setValue("out", std::string("sqrt(") + input.getValue("x", "1") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(math_exp)
{
    output.setValue("out", std::string("exp(") + input.getValue("x", "1") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(math_rands)
{
    auto args = std::string();

    args += input.getValue("min", "0");
    args += std::string(", ");
    args += input.getValue("max", "100");
    args += std::string(", ");
    args += input.getValue("n", "1");
    if (input.hasValue("seed")) {
	args += std::string(", ");
	args += input.getValue("seed", "0");
    }
    output.setValue("Geometry", std::string("rands(") + args + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(math_min)
{
    output.setValue("out", std::string("min(") + input.getValue("a", "0") + "," + input.getValue("b", "0") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(math_max)
{
    output.setValue("out", std::string("max(") + input.getValue("a", "0") + "," + input.getValue("b", "0") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(math_norm)
{
    output.setValue("out", std::string("norm(") + input.getValue("x", "0") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(math_cross)
{
    output.setValue("out", std::string("cross(") + input.getValue("a", "[1,0,0]") + "," + input.getValue("b", "[0,1,0]") + std::string(")"));
}


