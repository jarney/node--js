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
	    Connection(
		NodeId aFromNode,
		PortId aFromPort,
		NodeId aToNode,
		PortId aToPort
		);
	    ~Connection() = default;
	    
	    NodeId fromNode;
	    PortId fromPort;
	    NodeId toNode;
	    PortId toPort;
	    bool operator==(const Connection & other) const;
	    bool operator!=(const Connection & other) const;
	    ConnectionId getId() const;
	};
	
    }
}
