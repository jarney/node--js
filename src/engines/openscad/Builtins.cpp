#include "Builtins.hpp"
//#include "nodes/openscad/Builtins_helpers.hpp"

using namespace NodeJS::openscad;
using namespace NodeJS::core;

#define _OPENSCAD_NODE_REGISTER(name) ret->registerModel(std::move(f_##name()))

void
Builtins::registerDataModels()
{
#if 0    
    // Syntax
    _OPENSCAD_NODE_REGISTER(syntax_assign);        // parsed
//    _OPENSCAD_NODE_REGISTER(syntax_assign_list);
    _OPENSCAD_NODE_REGISTER(syntax_variable);      // parsed
//    _OPENSCAD_NODE_REGISTER(syntax_module);
    _OPENSCAD_NODE_REGISTER(syntax_function);
//    _OPENSCAD_NODE_REGISTER(syntax_include);
//    _OPENSCAD_NODE_REGISTER(syntax_use);

    // We put this in the syntax section because
    // we use this for custom modules and functions.
    ret->setCustomProcessor(Builtins::syntax_custom_node_processor);
    
    // Constants
    _OPENSCAD_NODE_REGISTER(const_true);           // parsed
    _OPENSCAD_NODE_REGISTER(const_false);          // parsed
    _OPENSCAD_NODE_REGISTER(const_int);            // parsed
    _OPENSCAD_NODE_REGISTER(const_float);          // parsed
    _OPENSCAD_NODE_REGISTER(const_string);         // parsed
    _OPENSCAD_NODE_REGISTER(const_undef);          // parsed

    // Operators
    _OPENSCAD_NODE_REGISTER(operator_add);         // parsed
    _OPENSCAD_NODE_REGISTER(operator_subtract);    // parsed
    _OPENSCAD_NODE_REGISTER(operator_multiply);    // parsed
    _OPENSCAD_NODE_REGISTER(operator_divide);      // parsed
    _OPENSCAD_NODE_REGISTER(operator_modulo);      // parsed
    _OPENSCAD_NODE_REGISTER(operator_exponentiate);// parsed
    _OPENSCAD_NODE_REGISTER(operator_lt);          // parsed
    _OPENSCAD_NODE_REGISTER(operator_leq);         // parsed
    _OPENSCAD_NODE_REGISTER(operator_eq);          // parsed
    _OPENSCAD_NODE_REGISTER(operator_neq);         // parsed
    _OPENSCAD_NODE_REGISTER(operator_geq);         // parsed
    _OPENSCAD_NODE_REGISTER(operator_gt);          // parsed
    _OPENSCAD_NODE_REGISTER(operator_and);         // parsed
    _OPENSCAD_NODE_REGISTER(operator_binary_and);  // parsed
    _OPENSCAD_NODE_REGISTER(operator_or);          // parsed
    _OPENSCAD_NODE_REGISTER(operator_binary_or);   // parsed
    _OPENSCAD_NODE_REGISTER(operator_binary_shl);  // parsed
    _OPENSCAD_NODE_REGISTER(operator_binary_shr);  // parsed
    _OPENSCAD_NODE_REGISTER(operator_not);         // parsed
    _OPENSCAD_NODE_REGISTER(operator_negate);      // parsed
    _OPENSCAD_NODE_REGISTER(operator_tilde);       // parsed
    
    // 2D
    _OPENSCAD_NODE_REGISTER(2d_circle);            // parsed
    _OPENSCAD_NODE_REGISTER(2d_square);            // parsed
    _OPENSCAD_NODE_REGISTER(2d_polygon);           // parsed
    _OPENSCAD_NODE_REGISTER(2d_text);              // parsed
    _OPENSCAD_NODE_REGISTER(2d_projection);        // parsed
    
    // 3D
    _OPENSCAD_NODE_REGISTER(3d_sphere);            // parsed
    _OPENSCAD_NODE_REGISTER(3d_cube);              // parsed
    _OPENSCAD_NODE_REGISTER(3d_cylinder);          // parsed
    _OPENSCAD_NODE_REGISTER(3d_polyhedron);        // parsed
    _OPENSCAD_NODE_REGISTER(3d_import);            // parsed
    _OPENSCAD_NODE_REGISTER(3d_linear_extrude);    // parsed
    _OPENSCAD_NODE_REGISTER(3d_rotate_extrude);    // parsed
    _OPENSCAD_NODE_REGISTER(3d_surface);           // parsed
    _OPENSCAD_NODE_REGISTER(3d_dxf_dim);           // parsed
    _OPENSCAD_NODE_REGISTER(3d_dxf_cross);         // parsed

    // Transformations
    _OPENSCAD_NODE_REGISTER(xform_translate);      // parsed
    _OPENSCAD_NODE_REGISTER(xform_rotate);         // parsed
    _OPENSCAD_NODE_REGISTER(xform_scale);          // parsed
    _OPENSCAD_NODE_REGISTER(xform_resize);         // parsed
    _OPENSCAD_NODE_REGISTER(xform_mirror);         // parsed
    _OPENSCAD_NODE_REGISTER(xform_multmatrix);     // parsed
    _OPENSCAD_NODE_REGISTER(xform_color);          // parsed
    _OPENSCAD_NODE_REGISTER(xform_offset);         // parsed
    _OPENSCAD_NODE_REGISTER(xform_hull);           // parsed
    _OPENSCAD_NODE_REGISTER(xform_fill);           // parsed
    _OPENSCAD_NODE_REGISTER(xform_minkowski);      // parsed

    // Lists
    _OPENSCAD_NODE_REGISTER(list_index);
    _OPENSCAD_NODE_REGISTER(list_get_xyz);
    _OPENSCAD_NODE_REGISTER(list_set_xyz);
    _OPENSCAD_NODE_REGISTER(list_get_xy);
    _OPENSCAD_NODE_REGISTER(list_set_xy);
    _OPENSCAD_NODE_REGISTER(list_get_rgba);
    _OPENSCAD_NODE_REGISTER(list_set_rgba);
    _OPENSCAD_NODE_REGISTER(list_set_range);

    // Boolean Operations
    _OPENSCAD_NODE_REGISTER(op_union);            // parsed
    _OPENSCAD_NODE_REGISTER(op_difference);       // parsed
    _OPENSCAD_NODE_REGISTER(op_intersection);     // parsed

    // Flow control
    _OPENSCAD_NODE_REGISTER(flow_for);
    _OPENSCAD_NODE_REGISTER(flow_intersection_for);
    _OPENSCAD_NODE_REGISTER(flow_if);
    _OPENSCAD_NODE_REGISTER(flow_let);
    _OPENSCAD_NODE_REGISTER(flow_comment);         // NO parse
    _OPENSCAD_NODE_REGISTER(flow_group);
    _OPENSCAD_NODE_REGISTER(flow_module_output);          // N/A
    _OPENSCAD_NODE_REGISTER(flow_function_output);          // N/A
    
    // Type Test functions
    _OPENSCAD_NODE_REGISTER(typetest_is_bool);     // parsed
    _OPENSCAD_NODE_REGISTER(typetest_is_string);   // parsed
    _OPENSCAD_NODE_REGISTER(typetest_is_num);      // parsed
    _OPENSCAD_NODE_REGISTER(typetest_is_function); // parsed
    _OPENSCAD_NODE_REGISTER(typetest_is_list);     // parsed
    _OPENSCAD_NODE_REGISTER(typetest_is_undef);    // parsed

    // Other
    _OPENSCAD_NODE_REGISTER(other_echo);
    _OPENSCAD_NODE_REGISTER(other_render);
    _OPENSCAD_NODE_REGISTER(other_children);
    _OPENSCAD_NODE_REGISTER(other_assert);
    
    // Functions
    _OPENSCAD_NODE_REGISTER(function_concat);      // parsed
    _OPENSCAD_NODE_REGISTER(function_lookup);      // parsed
    _OPENSCAD_NODE_REGISTER(function_str);         // parsed
    _OPENSCAD_NODE_REGISTER(function_chr);         // parsed
    _OPENSCAD_NODE_REGISTER(function_ord);         // parsed
    _OPENSCAD_NODE_REGISTER(function_search);      // parsed
    _OPENSCAD_NODE_REGISTER(function_version);     // parsed
    _OPENSCAD_NODE_REGISTER(function_version_num); // parsed
    _OPENSCAD_NODE_REGISTER(function_parent_module); // parsed
    
    // Math functions:
    _OPENSCAD_NODE_REGISTER(math_abs);             // parsed
    _OPENSCAD_NODE_REGISTER(math_sign);            // parsed
    _OPENSCAD_NODE_REGISTER(math_sin);             // parsed
    _OPENSCAD_NODE_REGISTER(math_cos);             // parsed
    _OPENSCAD_NODE_REGISTER(math_tan);             // parsed
    _OPENSCAD_NODE_REGISTER(math_acos);            // parsed
    _OPENSCAD_NODE_REGISTER(math_asin);            // parsed
    _OPENSCAD_NODE_REGISTER(math_atan);            // parsed
    _OPENSCAD_NODE_REGISTER(math_atan2);           // parsed
    _OPENSCAD_NODE_REGISTER(math_floor);           // parsed
    _OPENSCAD_NODE_REGISTER(math_round);           // parsed
    _OPENSCAD_NODE_REGISTER(math_ceil);            // parsed
    _OPENSCAD_NODE_REGISTER(math_ln);              // parsed
    _OPENSCAD_NODE_REGISTER(math_len);             // parsed
    _OPENSCAD_NODE_REGISTER(math_log);             // parsed
    _OPENSCAD_NODE_REGISTER(math_pow);             // parsed
    _OPENSCAD_NODE_REGISTER(math_sqrt);            // parsed
    _OPENSCAD_NODE_REGISTER(math_exp);             // parsed
    _OPENSCAD_NODE_REGISTER(math_rands);           // parsed
    _OPENSCAD_NODE_REGISTER(math_min);             // parsed
    _OPENSCAD_NODE_REGISTER(math_max);             // parsed
    _OPENSCAD_NODE_REGISTER(math_norm);            // parsed
    _OPENSCAD_NODE_REGISTER(math_cross);           // parsed
#endif
}

std::string
Builtins::joinArguments(std::vector<std::string> list)
{
    std::string out;
    bool first = true;

    for (const std::string & s : list) {
	if (!first) {
	    out += std::string(",");
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
