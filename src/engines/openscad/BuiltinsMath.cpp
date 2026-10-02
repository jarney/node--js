#include "nodes/openscad/Builtins.hpp"
#include "nodes/openscad/Builtins_helpers.hpp"

using namespace JNodes::openscad;
using namespace JNodes::core;

#define _OPENSCAD_NODE_CATEGORY Builtins::CATEGORY_MATH.getName()

UNARY_NODE(math, abs, _OPENSCAD_NODE_CATEGORY, "Absolute Value", "x", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("abs(") + input.getValue("x", "0") + std::string(")"));
}
UNARY_NODE(math, sign, _OPENSCAD_NODE_CATEGORY, "Sign", "x", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("sign(") + input.getValue("x", "0") + std::string(")"));
}
UNARY_NODE(math, sin, _OPENSCAD_NODE_CATEGORY, "Sine", "x", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("sin(") + input.getValue("x", "0") + std::string(")"));
}
UNARY_NODE(math, cos, _OPENSCAD_NODE_CATEGORY, "Cosine", "x", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("cos(") + input.getValue("x", "0") + std::string(")"));
}
UNARY_NODE(math, tan, _OPENSCAD_NODE_CATEGORY, "Tangent", "x", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("tan(") + input.getValue("x", "0") + std::string(")"));
}
UNARY_NODE(math, acos, _OPENSCAD_NODE_CATEGORY, "Arc Cosine", "x", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("acos(") + input.getValue("x", "1") + std::string(")"));
}
UNARY_NODE(math, asin, _OPENSCAD_NODE_CATEGORY, "Arc Sine", "x", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("asin(") + input.getValue("x", "0") + std::string(")"));
}
UNARY_NODE(math, atan, _OPENSCAD_NODE_CATEGORY, "Arc Tangent", "x", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("atan(") + input.getValue("x", "0") + std::string(")"));
}
BINARY_NODE(math, atan2, _OPENSCAD_NODE_CATEGORY, "Arc Tan(2)", "x", DATA_VARIABLE, "y", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("atan2(") + input.getValue("x", "0") + "," + input.getValue("y", "0") + std::string(")"));
}
UNARY_NODE(math, floor, _OPENSCAD_NODE_CATEGORY, "Floor", "x", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("floor(") + input.getValue("x", "0") + std::string(")"));
}
UNARY_NODE(math, round, _OPENSCAD_NODE_CATEGORY, "Round", "x", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("round(") + input.getValue("x", "0") + std::string(")"));
}
UNARY_NODE(math, ceil, _OPENSCAD_NODE_CATEGORY, "Ceiling", "x", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("ceil(") + input.getValue("x", "0") + std::string(")"));
}
UNARY_NODE(math, ln, _OPENSCAD_NODE_CATEGORY, "Natural Log", "x", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("ln(") + input.getValue("x", "2.718") + std::string(")"));
}
UNARY_NODE(math, len, _OPENSCAD_NODE_CATEGORY, "Length", "x", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("len(") + input.getValue("x", "0") + std::string(")"));
}
UNARY_NODE(math, log, _OPENSCAD_NODE_CATEGORY, "Log Base 10", "x", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("log(") + input.getValue("x", "10") + std::string(")"));
}
BINARY_NODE(math, pow, _OPENSCAD_NODE_CATEGORY, "Power", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("pow(") + input.getValue("a", "0") + "," + input.getValue("b", "0") + std::string(")"));
}
UNARY_NODE(math, sqrt, _OPENSCAD_NODE_CATEGORY, "Square Root", "x", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("sqrt(") + input.getValue("x", "1") + std::string(")"));
}
UNARY_NODE(math, exp, _OPENSCAD_NODE_CATEGORY, "Exponential(e)", "x", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("exp(") + input.getValue("x", "1") + std::string(")"));
}
void
Builtins::f_math_rands_process(const Node & node, const NodePortData & input, NodePortData & output)
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
BINARY_NODE(math, min, _OPENSCAD_NODE_CATEGORY, "Minimum", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("min(") + input.getValue("a", "0") + "," + input.getValue("b", "0") + std::string(")"));
}
BINARY_NODE(math, max, _OPENSCAD_NODE_CATEGORY, "Maximum", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("max(") + input.getValue("a", "0") + "," + input.getValue("b", "0") + std::string(")"));
}
UNARY_NODE(math, norm, _OPENSCAD_NODE_CATEGORY, "Norm", "x", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("norm(") + input.getValue("x", "0") + std::string(")"));
}
BINARY_NODE(math, cross, _OPENSCAD_NODE_CATEGORY, "Cross Product", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("cross(") + input.getValue("a", "[1,0,0]") + "," + input.getValue("b", "[0,1,0]") + std::string(")"));
}


