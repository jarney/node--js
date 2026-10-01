#include "node--js/Validator.hpp"
#include "node--js/NodeModule.hpp"

/**
 * Validates the semantics (meaning) of
 * a node module.
 * Several things need to line up in order
 * for a module to make 'semantic' sense.
 *
 * - All node type ports must point to valid DataType elements.
 * - All node type ports must have a unique identifier.
 * - All nodes must point to a valid node type.
 * - All node IDs must be unique.
 * - All connections must be between existing nodes and ports
 */

bool NodeJS::core::validate(const NodeModule & module, ValidationError & err)
{
    return true;
}
