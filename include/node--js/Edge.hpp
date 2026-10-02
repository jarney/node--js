#pragma once

#include "node--js/Node.hpp"

namespace NodeJS {
    namespace core {

	/**
	 * A connection ID is a unique 
	 * identifier (within a graph) for a
	 * connection between nodes.
	 */
	typedef std::string EdgeId;
	
	class Edge {
	public:
	    Edge(
		NodeId aFromNode,
		PortId aFromPort,
		NodeId aToNode,
		PortId aToPort
		);
	    ~Edge() = default;
	    
	    NodeId fromNode;
	    PortId fromPort;
	    NodeId toNode;
	    PortId toPort;
	    bool operator==(const Edge & other) const;
	    bool operator!=(const Edge & other) const;
	    EdgeId getId() const;
	};
	
    }
}
