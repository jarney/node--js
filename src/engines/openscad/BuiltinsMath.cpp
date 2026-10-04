#include "node--js/engines/openscad/Builtins.hpp"

using namespace NodeJS::openscad;
using namespace NodeJS::core;

void
Builtins::f_math_abs_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("abs(") + input.getValue("x", "0") + std::string(")"));
}
void
Builtins::f_math_sign_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("sign(") + input.getValue("x", "0") + std::string(")"));
}
void
Builtins::f_math_sin_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("sin(") + input.getValue("x", "0") + std::string(")"));
}
void
Builtins::f_math_cos_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("cos(") + input.getValue("x", "0") + std::string(")"));
}
void
Builtins::f_math_tan_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("tan(") + input.getValue("x", "0") + std::string(")"));
}
void
Builtins::f_math_acos_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("acos(") + input.getValue("x", "1") + std::string(")"));
}
void
Builtins::f_math_asin_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("asin(") + input.getValue("x", "0") + std::string(")"));
}
void
Builtins::f_math_atan_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("atan(") + input.getValue("x", "0") + std::string(")"));
}
void
Builtins::f_math_atan2_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("atan2(") + input.getValue("x", "0") + "," + input.getValue("y", "0") + std::string(")"));
}
void
Builtins::f_math_floor_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("floor(") + input.getValue("x", "0") + std::string(")"));
}
void
Builtins::f_math_round_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("round(") + input.getValue("x", "0") + std::string(")"));
}
void
Builtins::f_math_ceil_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("ceil(") + input.getValue("x", "0") + std::string(")"));
}
void
Builtins::f_math_ln_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("ln(") + input.getValue("x", "2.718") + std::string(")"));
}
void
Builtins::f_math_len_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("len(") + input.getValue("x", "0") + std::string(")"));
}
void
Builtins::f_math_log_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("log(") + input.getValue("x", "10") + std::string(")"));
}
void
Builtins::f_math_pow_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("pow(") + input.getValue("a", "0") + "," + input.getValue("b", "0") + std::string(")"));
}
void
Builtins::f_math_sqrt_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("sqrt(") + input.getValue("x", "1") + std::string(")"));
}
void
Builtins::f_math_exp_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("exp(") + input.getValue("x", "1") + std::string(")"));
}
void
Builtins::f_math_rands_process(const Node & node, const ConnectionData & input, ConnectionData & output)
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
void
Builtins::f_math_min_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("min(") + input.getValue("a", "0") + "," + input.getValue("b", "0") + std::string(")"));
}
void
Builtins::f_math_max_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("max(") + input.getValue("a", "0") + "," + input.getValue("b", "0") + std::string(")"));
}
void
Builtins::f_math_norm_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("norm(") + input.getValue("x", "0") + std::string(")"));
}
void
Builtins::f_math_cross_process(const Node & node, const ConnectionData & input, ConnectionData & output)
{
    output.setValue("out", std::string("cross(") + input.getValue("a", "[1,0,0]") + "," + input.getValue("b", "[0,1,0]") + std::string(")"));
}


