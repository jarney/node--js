#include "node--js/engines/openscad/Builtins.hpp"

using namespace NodeJS::openscad;
using namespace NodeJS::core;

#define _OPENSCAD_PROCESSOR_DEF(name)                  \
    void Builtins::NodeProcessor_##name##_fn::process(      \
	const Node & node,                             \
	const ConnectionData & input,                  \
	ConnectionData & output                        \
	)

_OPENSCAD_PROCESSOR_DEF(const_true)
{
    output.setValue("value", std::string("true"));
}
_OPENSCAD_PROCESSOR_DEF(const_false)
{
    output.setValue("value", std::string("false"));
}
_OPENSCAD_PROCESSOR_DEF(const_undef)
{
    output.setValue("value", std::string("undef"));
}

////////////////////////////////////////
// Integer Constant
////////////////////////////////////////
_OPENSCAD_PROCESSOR_DEF(const_int)
{
    output.setValue("value", node.getData().getValue("value", "0"));
}

////////////////////////////////////////
// Float Constant
////////////////////////////////////////
_OPENSCAD_PROCESSOR_DEF(const_float)
{
    output.setValue("value", node.getData().getValue("value", "0.0"));
}

////////////////////////////////////////
// String Constant
////////////////////////////////////////
_OPENSCAD_PROCESSOR_DEF(const_string)
{
    fprintf(stderr, "Processing string to %s\n", node.getData().getValue("value").c_str());
    output.setValue(
	"value",
	std::string("\"") +
	node.getData().getValue("value", "") + 
	std::string("\"")
	);
}

