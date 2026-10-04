#include "node--js/engines/openscad/Builtins.hpp"

using namespace NodeJS::openscad;
using namespace NodeJS::core;

#define _OPENSCAD_PROCESSOR_DEF(name)                  \
    void Builtins::NodeProcessor_##name##_fn::process(      \
	const Node & node,                             \
	const ConnectionData & input,                  \
	ConnectionData & output                        \
	)

_OPENSCAD_PROCESSOR_DEF(typetest_is_bool)
{
    output.setValue("out", std::string("is_bool(") + input.getValue("x", "true") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(typetest_is_string)
{
    output.setValue("out", std::string("is_string(") + input.getValue("x", "true") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(typetest_is_num)
{
    output.setValue("out", std::string("is_num(") + input.getValue("x", "true") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(typetest_is_function)
{
    output.setValue("out", std::string("is_function(") + input.getValue("x", "true") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(typetest_is_list)
{
    output.setValue("out", std::string("is_list(") + input.getValue("x", "true") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(typetest_is_undef)
{
    output.setValue("out", std::string("is_undef(") + input.getValue("x", "true") + std::string(")"));
}
