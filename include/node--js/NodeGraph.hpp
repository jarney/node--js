#pragma once

#include "node--js/Connection.hpp"
#include "node--js/Node.hpp"

#include <optional>

namespace NodeJS {
    namespace core {

	class NodeGraph {
	public:
	    NodeGraph();
	    ~NodeGraph();
	    
	    /**
	     * Creates a new node of the given type, attempting to use
	     * the given string as the node ID.  If this
	     * would violate the node's uniqueness, an
	     * identifier is added in order to disambiguate
	     * and ensure each node gets a unique id.
	     *
	     * The node's internal data is initialized with
	     * the given data.
	     */
	    Node & newNode(
		const NodeType & aNodeType,
		const std::string & aNodeIdCandidate,
		const ConnectionData & aConnectionData
		);

	    /**
	     * Returns the node associated with the
	     * given ID.  Note that this is only valid
	     * if the given node id exists.  If not, it
	     * is undefined behavior.  The only way
	     * to call this function is with a node
	     * that comes from a valid connection
	     * or from a node that definitely exists.
	     */
	    Node * getNode(NodeId aNodeId) const;

	    /**
	     * Returns whether or not the getNode
	     * would be successful if called.
	     */
	    bool hasNode(NodeId aNodeId) const;
	    
	    /**
	     * Attempts to create a new connection between
	     * the given node and ports.  If the connection
	     * already existed, we just return that.  If not,
	     * a new connection id is created and returned.
	     * If one of the nodes or ports does not exist,
	     * then the optional is returned without a
	     * connection id indicating that it could not be created.
	     */
	    std::optional<ConnectionId> newConnection(
		NodeId aFromNode,
		PortId aFromPort,
		NodeId aToNode,
		PortId aToPort
		);

	    /**
	     * Returns a list of the connections originating
	     * from the given node id.  That is, all connections
	     * with a 'from node' equal to the given node.
	     */
	    std::vector<const Connection*> getConnectionsFrom(NodeId aNodeId) const;

	    /**
	     * Return a list of the connections terminating
	     * at the given node id.  That is, all connections
	     * with a 'to node' equal to the given node.
	     */
	    std::vector<const Connection*> getConnectionsTo(NodeId aNodeId) const;

	private:

	    std::string findNewNodeId(std::string aNodeIdCandidate);
	    
	    // The core of a graph is the nodes and edges.
	    std::map<NodeId, std::unique_ptr<Node>> mNodes;
	    
	    std::map<ConnectionId, Connection> mEdges;
	    std::map<NodeId, std::vector<Connection*>> mConnectionsByFromNode;
	    std::map<NodeId, std::vector<Connection*>> mConnectionsByToNode;
	};
    }
}
