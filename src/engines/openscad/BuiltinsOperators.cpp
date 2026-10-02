#include "nodes/openscad/Builtins.hpp"
#include "nodes/openscad/Builtins_helpers.hpp"

using namespace JNodes::openscad;
using namespace JNodes::core;

#define _OPENSCAD_NODE_CATEGORY Builtins::CATEGORY_OPERATOR.getName()

BINARY_NODE(operator, add, _OPENSCAD_NODE_CATEGORY, "Add", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + "+" + input.getValue("b", "0") + std::string(")"));
}
BINARY_NODE(operator, subtract, _OPENSCAD_NODE_CATEGORY, "Subtract", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + "-" + input.getValue("b", "0") + std::string(")"));
}
BINARY_NODE(operator, multiply, _OPENSCAD_NODE_CATEGORY, "Multiply", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + "*" + input.getValue("b", "0") + std::string(")"));
}
BINARY_NODE(operator, divide, _OPENSCAD_NODE_CATEGORY, "Divide", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + "/" + input.getValue("b", "0") + std::string(")"));
}
BINARY_NODE(operator, modulo, _OPENSCAD_NODE_CATEGORY, "Modulo", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + "%" + input.getValue("b", "0") + std::string(")"));
}
BINARY_NODE(operator, exponentiate, _OPENSCAD_NODE_CATEGORY, "Exponentiate", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + "^" + input.getValue("b", "0") + std::string(")"));
}
BINARY_NODE(operator, lt, _OPENSCAD_NODE_CATEGORY, "Less Than", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + "<" + input.getValue("b", "0") + std::string(")"));
}
BINARY_NODE(operator, leq, _OPENSCAD_NODE_CATEGORY, "Less Than or Equal", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + "<=" + input.getValue("b", "0") + std::string(")"));
}
BINARY_NODE(operator, eq, _OPENSCAD_NODE_CATEGORY, "Equal", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + "==" + input.getValue("b", "0") + std::string(")"));
}
BINARY_NODE(operator, neq, _OPENSCAD_NODE_CATEGORY, "Not Equal", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + "!=" + input.getValue("b", "0") + std::string(")"));
}
BINARY_NODE(operator, geq, _OPENSCAD_NODE_CATEGORY, "Greater Than or Equal", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + ">=" + input.getValue("b", "0") + std::string(")"));
}
BINARY_NODE(operator, gt, _OPENSCAD_NODE_CATEGORY, "Greater Than", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + ">" + input.getValue("b", "0") + std::string(")"));
}
BINARY_NODE(operator, and, _OPENSCAD_NODE_CATEGORY, "And", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + ")&&(" + input.getValue("b", "0") + std::string(")"));
}
BINARY_NODE(operator, binary_and, _OPENSCAD_NODE_CATEGORY, "Bitwise And", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + ")&(" + input.getValue("b", "0") + std::string(")"));
}
BINARY_NODE(operator, or, _OPENSCAD_NODE_CATEGORY, "Or", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + ")||(" + input.getValue("b", "0") + std::string(")"));
}
BINARY_NODE(operator, binary_or, _OPENSCAD_NODE_CATEGORY, "Bitwise Or", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + ")|(" + input.getValue("b", "0") + std::string(")"));
}
BINARY_NODE(operator, binary_shl, _OPENSCAD_NODE_CATEGORY, "Bitwise Shift Left", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + ")<<(" + input.getValue("b", "0") + std::string(")"));
}
BINARY_NODE(operator, binary_shr, _OPENSCAD_NODE_CATEGORY, "Bitwise Shift Right", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + ")>>(" + input.getValue("b", "0") + std::string(")"));
}
UNARY_NODE(operator, not, _OPENSCAD_NODE_CATEGORY, "Not", "a", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("!(") + input.getValue("a", "0") + std::string(")"));
}
UNARY_NODE(operator, negate, _OPENSCAD_NODE_CATEGORY, "Negate", "a", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("-(") + input.getValue("a", "0") + std::string(")"));
}
UNARY_NODE(operator, tilde, _OPENSCAD_NODE_CATEGORY, "Binary Not", "a", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("~(") + input.getValue("a", "0") + std::string(")"));
}
