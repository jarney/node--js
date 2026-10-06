#include "node--js/engines/openscad/Builtins.hpp"

using namespace NodeJS::openscad;
using namespace NodeJS::core;

#define _OPENSCAD_NODE_REGISTER(name, val) \
    processor.setNativeImpl(name, std::make_unique<NodeProcessor_##val##_fn>());

void
Builtins::registerProcessors(Processor & processor)
{
    // Syntax
    _OPENSCAD_NODE_REGISTER("assign", syntax_assign);
//    _OPENSCAD_NODE_REGISTER(syntax_assign_list);
    _OPENSCAD_NODE_REGISTER("variable", syntax_variable);
//    _OPENSCAD_NODE_REGISTER(syntax_module);
    _OPENSCAD_NODE_REGISTER("function", syntax_function);
    _OPENSCAD_NODE_REGISTER("function-call", syntax_function_call);
//    _OPENSCAD_NODE_REGISTER(syntax_include);
//    _OPENSCAD_NODE_REGISTER(syntax_use);

    _OPENSCAD_NODE_REGISTER("custom_node", syntax_custom_node);

    // Constants
    _OPENSCAD_NODE_REGISTER("true", const_true);
    _OPENSCAD_NODE_REGISTER("false", const_false);
    _OPENSCAD_NODE_REGISTER("const_int", const_int);
    _OPENSCAD_NODE_REGISTER("const_float", const_float);
    _OPENSCAD_NODE_REGISTER("const_string", const_string);
    _OPENSCAD_NODE_REGISTER("undef", const_string);

    // Operators
    _OPENSCAD_NODE_REGISTER("add", operator_add);         // parsed
    _OPENSCAD_NODE_REGISTER("subtract", operator_subtract);    // parsed
    _OPENSCAD_NODE_REGISTER("multiply", operator_multiply);    // parsed
    _OPENSCAD_NODE_REGISTER("divide", operator_divide);      // parsed
    _OPENSCAD_NODE_REGISTER("modulo", operator_modulo);      // parsed
    _OPENSCAD_NODE_REGISTER("exponentiate", operator_exponentiate);// parsed
    _OPENSCAD_NODE_REGISTER("lt", operator_lt);          // parsed
    _OPENSCAD_NODE_REGISTER("leq", operator_leq);         // parsed
    _OPENSCAD_NODE_REGISTER("eq", operator_eq);          // parsed
    _OPENSCAD_NODE_REGISTER("neq", operator_neq);         // parsed
    _OPENSCAD_NODE_REGISTER("geq", operator_geq);         // parsed
    _OPENSCAD_NODE_REGISTER("gt", operator_gt);          // parsed
    _OPENSCAD_NODE_REGISTER("and", operator_and);         // parsed
    _OPENSCAD_NODE_REGISTER("binary_and", operator_binary_and);  // parsed
    _OPENSCAD_NODE_REGISTER("or", operator_or);          // parsed
    _OPENSCAD_NODE_REGISTER("binary_or", operator_binary_or);   // parsed
    _OPENSCAD_NODE_REGISTER("binary_shl", operator_binary_shl);  // parsed
    _OPENSCAD_NODE_REGISTER("binary_shr", operator_binary_shr);  // parsed
    _OPENSCAD_NODE_REGISTER("not", operator_not);         // parsed
    _OPENSCAD_NODE_REGISTER("negate", operator_negate);      // parsed
    _OPENSCAD_NODE_REGISTER("tilde", operator_tilde);       // parsed
    
    // 2D
    _OPENSCAD_NODE_REGISTER("circle", 2d_circle);            // parsed
    _OPENSCAD_NODE_REGISTER("square", 2d_square);            // parsed
    _OPENSCAD_NODE_REGISTER("polygon", 2d_polygon);           // parsed
    _OPENSCAD_NODE_REGISTER("text", 2d_text);              // parsed
    _OPENSCAD_NODE_REGISTER("projection", 2d_projection);        // parsed

    // 3D
    _OPENSCAD_NODE_REGISTER("sphere", 3d_sphere);            // parsed
    _OPENSCAD_NODE_REGISTER("cube", 3d_cube);              // parsed
    _OPENSCAD_NODE_REGISTER("cylinder", 3d_cylinder);          // parsed
    _OPENSCAD_NODE_REGISTER("polyhedron", 3d_polyhedron);        // parsed
    _OPENSCAD_NODE_REGISTER("import", 3d_import);            // parsed
    _OPENSCAD_NODE_REGISTER("linear_extrude", 3d_linear_extrude);    // parsed
    _OPENSCAD_NODE_REGISTER("rotate_extrude", 3d_rotate_extrude);    // parsed
    _OPENSCAD_NODE_REGISTER("surface", 3d_surface);           // parsed
    _OPENSCAD_NODE_REGISTER("dxf_dim", 3d_dxf_dim);           // parsed
    _OPENSCAD_NODE_REGISTER("dxf_cross", 3d_dxf_cross);         // parsed

    // Transformations
    _OPENSCAD_NODE_REGISTER("translate", xform_translate);      // parsed
    _OPENSCAD_NODE_REGISTER("rotate", xform_rotate);         // parsed
    _OPENSCAD_NODE_REGISTER("scale", xform_scale);          // parsed
    _OPENSCAD_NODE_REGISTER("resize", xform_resize);         // parsed
    _OPENSCAD_NODE_REGISTER("mirror", xform_mirror);         // parsed
    _OPENSCAD_NODE_REGISTER("multmatrix", xform_multmatrix);     // parsed
    _OPENSCAD_NODE_REGISTER("color", xform_color);          // parsed
    _OPENSCAD_NODE_REGISTER("offset", xform_offset);         // parsed
    _OPENSCAD_NODE_REGISTER("hull", xform_hull);           // parsed
    _OPENSCAD_NODE_REGISTER("fill", xform_fill);           // parsed
    _OPENSCAD_NODE_REGISTER("minkowski", xform_minkowski);      // parsed

    // Lists
    _OPENSCAD_NODE_REGISTER("__builtin_list_index", list_index);
    _OPENSCAD_NODE_REGISTER("__builtin_list_get_xyz", list_get_xyz);
    _OPENSCAD_NODE_REGISTER("__builtin_list_set_xyz", list_set_xyz);
    _OPENSCAD_NODE_REGISTER("__builtin_list_get_xy", list_get_xy);
    _OPENSCAD_NODE_REGISTER("__builtin_list_set_xy", list_set_xy);
    _OPENSCAD_NODE_REGISTER("__builtin_list_get_rgba", list_get_rgba);
    _OPENSCAD_NODE_REGISTER("__builtin_list_set_rgba", list_set_rgba);
    _OPENSCAD_NODE_REGISTER("__builtin_list_set_range", list_set_range);

    // Boolean Operations
    _OPENSCAD_NODE_REGISTER("union", op_union);            // parsed
    _OPENSCAD_NODE_REGISTER("difference", op_difference);       // parsed
    _OPENSCAD_NODE_REGISTER("intersection", op_intersection);     // parsed

    // Flow control
    _OPENSCAD_NODE_REGISTER("for", flow_for);
    _OPENSCAD_NODE_REGISTER("intersection_for", flow_intersection_for);
    _OPENSCAD_NODE_REGISTER("if", flow_if);
    _OPENSCAD_NODE_REGISTER("let", flow_let);
    //_OPENSCAD_NODE_REGISTER("comment", flow_comment);         // There isn't a processor for this node, so we just skip it.
    _OPENSCAD_NODE_REGISTER("group", flow_group);
    _OPENSCAD_NODE_REGISTER("module_output", flow_module_output);          // N/A
    _OPENSCAD_NODE_REGISTER("function_output", flow_function_output);          // N/A
    
    // Type Test functions
    _OPENSCAD_NODE_REGISTER("is_bool", typetest_is_bool);     // parsed
    _OPENSCAD_NODE_REGISTER("is_string", typetest_is_string);   // parsed
    _OPENSCAD_NODE_REGISTER("is_num", typetest_is_num);      // parsed
    _OPENSCAD_NODE_REGISTER("is_function", typetest_is_function); // parsed
    _OPENSCAD_NODE_REGISTER("is_list", typetest_is_list);     // parsed
    _OPENSCAD_NODE_REGISTER("is_undef", typetest_is_undef);    // parsed

    // Other
    _OPENSCAD_NODE_REGISTER("echo", other_echo);
    _OPENSCAD_NODE_REGISTER("render", other_render);
    _OPENSCAD_NODE_REGISTER("children", other_children);
    _OPENSCAD_NODE_REGISTER("assert", other_assert);
    
    // Functions
    _OPENSCAD_NODE_REGISTER("concat", function_concat);      // parsed
    _OPENSCAD_NODE_REGISTER("lookup", function_lookup);      // parsed
    _OPENSCAD_NODE_REGISTER("str", function_str);         // parsed
    _OPENSCAD_NODE_REGISTER("chr", function_chr);         // parsed
    _OPENSCAD_NODE_REGISTER("ord", function_ord);         // parsed
    _OPENSCAD_NODE_REGISTER("search", function_search);      // parsed
    _OPENSCAD_NODE_REGISTER("version", function_version);     // parsed
    _OPENSCAD_NODE_REGISTER("version_num", function_version_num); // parsed
    _OPENSCAD_NODE_REGISTER("parent_module", function_parent_module); // parsed
    
    // Math functions:
    _OPENSCAD_NODE_REGISTER("abs", math_abs);             // parsed
    _OPENSCAD_NODE_REGISTER("sign", math_sign);            // parsed
    _OPENSCAD_NODE_REGISTER("sin", math_sin);             // parsed
    _OPENSCAD_NODE_REGISTER("cos", math_cos);             // parsed
    _OPENSCAD_NODE_REGISTER("tan", math_tan);             // parsed
    _OPENSCAD_NODE_REGISTER("acos", math_acos);            // parsed
    _OPENSCAD_NODE_REGISTER("asin", math_asin);            // parsed
    _OPENSCAD_NODE_REGISTER("atan", math_atan);            // parsed
    _OPENSCAD_NODE_REGISTER("atan2", math_atan2);           // parsed
    _OPENSCAD_NODE_REGISTER("floor", math_floor);           // parsed
    _OPENSCAD_NODE_REGISTER("round", math_round);           // parsed
    _OPENSCAD_NODE_REGISTER("ceil", math_ceil);            // parsed
    _OPENSCAD_NODE_REGISTER("ln", math_ln);              // parsed
    _OPENSCAD_NODE_REGISTER("len", math_len);             // parsed
    _OPENSCAD_NODE_REGISTER("log", math_log);             // parsed
    _OPENSCAD_NODE_REGISTER("pow", math_pow);             // parsed
    _OPENSCAD_NODE_REGISTER("sqrt", math_sqrt);            // parsed
    _OPENSCAD_NODE_REGISTER("exp", math_exp);             // parsed
    _OPENSCAD_NODE_REGISTER("rands", math_rands);           // parsed
    _OPENSCAD_NODE_REGISTER("min", math_min);             // parsed
    _OPENSCAD_NODE_REGISTER("max", math_max);             // parsed
    _OPENSCAD_NODE_REGISTER("norm", math_norm);            // parsed
    _OPENSCAD_NODE_REGISTER("cross", math_cross);           // parsed
}

std::string
Builtins::joinArguments(std::vector<std::string> list)
{
    std::string out;
    bool first = true;

    for (const std::string & s : list) {
	if (!first) {
	    out += std::string(",");
	}
	else {
	    first = false;
	}
	out += s;
    }
    return out;
}

void
Builtins::conditionalArg(
    std::vector<std::string> & args,
    const ConnectionData & input,
    const Node & node,
    std::string key,
    std::string default_value)
{
    if (input.hasValue(key)) {
	args.push_back(key + std::string("=") + input.getValue(key, ""));
    }
    else if (node.getData().hasValue(key)) {
	args.push_back(key + std::string("=") + node.getData().getValue(key, ""));
    }
    else {
	args.push_back(key + std::string("=") + default_value);
    }
}

void
Builtins::conditionalArg(
    std::vector<std::string> & args,
    const ConnectionData & input,
    const Node & node,
    std::string key
    )
{
    if (input.hasValue(key)) {
	args.push_back(key + std::string("=") + input.getValue(key, ""));
    }
    else if (node.getData().hasValue(key)) {
	args.push_back(key + std::string("=") + node.getData().getValue(key, ""));
    }
}

void
Builtins::optionalUnnamed(
    std::vector<std::string> & args,
    const ConnectionData & input,
    const Node & node,
    std::string key
    )
{
    if (input.hasValue(key)) {
	args.push_back(input.getValue(key));
    }
}

void
Builtins::optionalNamed(
    std::vector<std::string> & args,
    const ConnectionData & input,
    const Node & node,
    std::string argname,
    std::string key
    )
{
    if (input.hasValue(key)) {
	args.push_back(argname + std::string("=") + input.getValue(key));
    }
}
