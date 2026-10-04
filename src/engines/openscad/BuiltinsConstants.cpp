#include "node--js/engines/openscad/Builtins.hpp"

using namespace NodeJS::openscad;
using namespace NodeJS::core;

void
Builtins::f_const_true_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("value", std::string("true"));
}
void
Builtins::f_const_false_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("value", std::string("false"));
}
void
Builtins::f_const_undef_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("value", std::string("undef"));
}

////////////////////////////////////////
// Integer Constant
////////////////////////////////////////
void
Builtins::f_const_int_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("value", node.getData().getValue("value", "0"));
}

////////////////////////////////////////
// Float Constant
////////////////////////////////////////
void
Builtins::f_const_float_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("value", node.getData().getValue("value", "0.0"));
}

////////////////////////////////////////
// String Constant
////////////////////////////////////////
void
Builtins::f_const_string_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    fprintf(stderr, "Processing string to %s\n", node.getData().getValue("value").c_str());
    output.setValue(
	"value",
	std::string("\"") +
	node.getData().getValue("value", "") + 
	std::string("\"")
	);
}

