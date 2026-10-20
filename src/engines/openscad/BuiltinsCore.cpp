#include "node--js/engines/openscad/Builtins.hpp"
#include "node--js/NodeGraph.hpp"
#include "node--js/NodeModule.hpp"

using namespace NodeJS::openscad;
using namespace NodeJS::core;

#define _OPENSCAD_PROCESSOR_DEF(name)                  \
    void Builtins::NodeProcessor_##name##_fn::process( \
	Processor & processor,                         \
	const Node & node,                             \
	const ConnectionData & input,                  \
	ConnectionData & output                        \
	)

////////////////////////////////////////
// Core
////////////////////////////////////////
_OPENSCAD_PROCESSOR_DEF(core_string_to_variable)
{ output.setValue("out", input.getValue("in")); }

_OPENSCAD_PROCESSOR_DEF(core_variable_to_string)
{ output.setValue("out", input.getValue("in")); }

_OPENSCAD_PROCESSOR_DEF(core_string_to_module_instantiation)
{ output.setValue("out", input.getValue("in")); }

_OPENSCAD_PROCESSOR_DEF(core_string_to_assignment)
{ output.setValue("out", input.getValue("in")); }

