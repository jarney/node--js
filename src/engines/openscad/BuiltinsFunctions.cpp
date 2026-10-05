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
    output.setValue("out", std::string("concat(") + input.getValue("a", "0") + "," + input.getValue("b", "0") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(function_lookup)
{
    output.setValue("out", std::string("lookup(") + input.getValue("value", "0") + ", " + input.getValue("table", "0") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(function_str)
{
    output.setValue("out", std::string("str(") + input.getValue("x", "true") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(function_chr)
{
    output.setValue("out", std::string("chr(") + input.getValue("x", "true") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(function_ord)
{
    output.setValue("out", std::string("ord(") + input.getValue("x", "true") + std::string(")"));
}
_OPENSCAD_PROCESSOR_DEF(function_search)
{
    output.setValue("out", std::string("search(") + input.getValue("needle", "0") + ", " + input.getValue("haystack", "[]") + std::string(")"));
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
    output.setValue("out", std::string("parent_module(") + input.getValue("index", "0") + std::string(")"));
}

