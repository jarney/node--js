#include <string>

namespace NodeJS {
    namespace core {
	class NodeModule;

	/**
	 * Each of these enumerated values give
	 * a reason that the given node module does not
	 * pass validation.  Graphs that do not pass
	 * validation are not valid programs for the purpose
	 * of running the compile process on them.
	 */
	typedef enum {
	    /**
	     * Indicates that the graph is attempting to use
	     * a node with a type that has not been defined.
	     */
	    ERROR_NO_NODE_TYPE,

	    /**
	     * Indicates that there is more than
	     * one node type defined with the same name.
	     */
	    ERROR_DUPLICATE_NODE_TYPE,

	    /**
	     * Indicates that there is more than
	     * one edge of the given type.
	     */
	    ERROR_DUPLICATE_EDGE,

	    /**
	     * Indicates that the edge
	     * goes to an invalid port.
	     */
	    ERROR_EDGE_TO_INVALID_PORT,

	    /**
	     * Indicates that the edge
	     * comes from an invalid port.
	     */
	    ERROR_EDGE_FROM_INVALID_PORT,

	    /**
	     * Indicates that the data type
	     * of the connected ports does not match.
	     */
	    ERROR_EDGE_WRONG_DATA_TYPE,
	    
	    /**
	     * Indicates that there are
	     * more than one edge connected to
	     * this port, but the port indicates
	     * that it only supports a single edge.
	     */
	    ERROR_PORT_TOO_MANY_EDGES
	} ErrorType;
	
	class ValidationError {
	public:
	    ValidationError() = default;
	    virtual ~ValidationError() = default;
	    virtual void reportError(ErrorType type, std::string message) = 0;
	};

	
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
	bool validate(const NodeModule & module, ValidationError & err);
    }
}
