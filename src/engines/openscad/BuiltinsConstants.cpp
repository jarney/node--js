#include "nodes/openscad/Builtins.hpp"
#include "nodes/openscad/Builtins_helpers.hpp"

#include <QtWidgets/QLineEdit>
#include <QIntValidator>
#include <QDoubleValidator>

using namespace JNodes::openscad;
using namespace JNodes::core;

#define _OPENSCAD_NODE_CATEGORY Builtins::CATEGORY_CONST.getName()

NONARY_NODE(const, true, _OPENSCAD_NODE_CATEGORY, "True", "value", DATA_VARIABLE)
{
    output.setValue("value", std::string("true"));
}
NONARY_NODE(const, false, _OPENSCAD_NODE_CATEGORY, "False", "value", DATA_VARIABLE)
{
    output.setValue("value", std::string("false"));
}
NONARY_NODE(const, undef, _OPENSCAD_NODE_CATEGORY, "Undefined", "value", DATA_VARIABLE)
{
    output.setValue("value", std::string("undef"));
}

////////////////////////////////////////
// Integer Constant
////////////////////////////////////////
void
Builtins::f_const_int_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    output.setValue("value", node.getValue("value", "0"));
}
void
Builtins::f_const_int_initializer(Node & node)
{
    if (!node.hasValue("value")) {
	node.setValue("value", "0");
    }
}

////////////////////////////////////////
// Float Constant
////////////////////////////////////////
void
Builtins::f_const_float_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    output.setValue("value", node.getValue("value", "0.0"));
}
void
Builtins::f_const_float_initializer(Node & node)
{
    if (!node.hasValue("value")) {
	node.setValue("value", "0.0");
    }
}

////////////////////////////////////////
// String Constant
////////////////////////////////////////
void
Builtins::f_const_string_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    output.setValue(
	"value",
	std::string("\"") +
	node.getValue("value", "") + 
	std::string("\"")
	);
}

void
Builtins::f_const_string_initializer(Node & node)
{
    if (!node.hasValue("value")) {
	node.setValue("value", "");
    }
}
