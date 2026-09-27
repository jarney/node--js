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

	    const std::string & getId() const;
	    void setId(std::string id);
	    
	    
	    /**
	     * Returns the visibility of this node type.
	     * This determines whether the node type is
	     * visible outside the scope of this module
	     * or not.  This is useful for organizing
	     * code into modules where part of the
	     * impelementation is hidden.
	     */
	    NodeJS::core::NodeType::Visibility getVisibility(void) const;

	    void setVisibility(NodeJS::core::NodeType::Visibility visibility);

	    /**
	     * This returns the implementation type
	     * for this type.  Some types may be
	     * implemented on the underlying runtime
	     * rather than being implemented in terms
	     * of other nodes as a graph.
	     */
	    NodeJS::core::NodeType::Implementation getImplementation(void) const;
	    void setImplementation(NodeJS::core::NodeType::Implementation impl);
	    
	private:
	    std::string mId;
	    Visibility mVisibility;
	    Implementation mImplementation;
	    std::map<std::string, NodePort> mInputs;
	    std::map<std::string, NodePort> mOutputs;
	};
    }
}
