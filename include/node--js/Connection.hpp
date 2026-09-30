#pragma once

#include "node--js/Connection.hpp"
#include "node--js/Node.hpp"

namespace NodeJS {
    namespace core {

	/**
	 * A connection ID is a unique 
	 * identifier (within a graph) for a
	 * connection between nodes.
	 */
	typedef std::string ConnectionId;
	
	class Connection {
	public:
	    NodeId fromNode;
	    PortId fromPortId;
	    NodeId toNode;
	    PortId toPortId;
	    bool operator==(const Connection & other);
	};
	
    }
}
