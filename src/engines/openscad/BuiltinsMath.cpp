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

_OPENSCAD_PROCESSOR_DEF(math_atan2)
{
    std::vector<std::string> args;
    optionalUnnamed(args, input, node, "x");
    optionalUnnamed(args, input, node, "y");
    output.setValue("out", std::string("atan2(") + joinArguments(args) + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(math_pow)
{
    std::vector<std::string> args;
    optionalUnnamed(args, input, node, "a");
    optionalUnnamed(args, input, node, "b");
    output.setValue("out", std::string("pow(") + joinArguments(args) + std::string(")"));
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
_OPENSCAD_PROCESSOR_DEF(math_cross)
{
    std::vector<std::string> args;
    optionalUnnamed(args, input, node, "a");
    optionalUnnamed(args, input, node, "b");
    output.setValue("out", std::string("cross(") + joinArguments(args) + std::string(")"));
}


