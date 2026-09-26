#pragma once

#include <string>
#include <map>

#include "node--js/NodePort.hpp"

namespace NodeJS {
    namespace core {
	class NodeType {
	public:

	    typedef enum {
		PUBLIC,
		PRIVATE
	    } Visibility;

	    typedef enum {
		NATIVE,
		GRAPH
	    } Implementation;
	    
	    NodeType() = default;
	    ~NodeType() = default;

	    /**
	     * Returns the visibility of this node type.
	     * This determines whether the node type is
	     * visible outside the scope of this module
	     * or not.  This is useful for organizing
	     * code into modules where part of the
	     * impelementation is hidden.
	     */
	    Visibility getVisibility(void) const;

	    /**
	     * This returns the implementation type
	     * for this type.  Some types may be
	     * implemented on the underlying runtime
	     * rather than being implemented in terms
	     * of other nodes as a graph.
	     */
	    Implementation getImplementation(void) const;
	    
	private:
	    std::map<std::string, NodePort> mInputs;
	    std::map<std::string, NodePort> mOutputs;
	};
    }
}
