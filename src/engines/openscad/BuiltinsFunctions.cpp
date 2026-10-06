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

_OPENSCAD_PROCESSOR_DEF(function_concat)
{
    std::vector<std::string> args;
    optionalUnnamed(args, input, node, "a");
    optionalUnnamed(args, input, node, "b");
    output.setValue("out", std::string("concat(") + joinArguments(args) + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(function_lookup)
{
    std::vector<std::string> args;
    optionalUnnamed(args, input, node, "value");
    optionalUnnamed(args, input, node, "table");
    output.setValue("out", std::string("lookup(") + joinArguments(args) + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(function_str)
{
    output.setValue("out", std::string("str(") + input.getValue("x") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(function_chr)
{
    output.setValue("out", std::string("chr(") + input.getValue("x") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(function_ord)
{
    output.setValue("out", std::string("ord(") + input.getValue("x") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(function_search)
{
    std::vector<std::string> args;
    optionalUnnamed(args, input, node, "needle");
    optionalUnnamed(args, input, node, "haystack");
    output.setValue("out", std::string("search(") + joinArguments(args) + std::string(")"));
}

////////////////////////////////////////
// Version
////////////////////////////////////////
_OPENSCAD_PROCESSOR_DEF(function_version)
{
    output.setValue("value", std::string("version()"));
}
////////////////////////////////////////
// Version Number
////////////////////////////////////////
_OPENSCAD_PROCESSOR_DEF(function_version_num)
{
    output.setValue("value", std::string("version_num()"));
}

////////////////////////////////////////
// Parent Module
////////////////////////////////////////
_OPENSCAD_PROCESSOR_DEF(function_parent_module)
{
    output.setValue("out", std::string("parent_module(") + input.getValue("index") + std::string(")"));
}

