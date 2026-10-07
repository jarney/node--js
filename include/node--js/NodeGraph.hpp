#pragma once

#include "node--js/Metadata.hpp"
#include "node--js/Edge.hpp"
#include "node--js/Node.hpp"

#include <optional>
#include <set>

namespace NodeJS {
    namespace core {

	class Scope {
	    // List of other
	    // foreign namespaces that
	    // can be resolved from here.
	    // The scope is searched in order
	    // when resolving node types.
	};

	class NodeModule;

	typedef std::string GroupId;
	
	class Group {
	public:
	    Group() = default;
	    ~Group() = default;

	    void addNode(NodeId aNodeId);
	    
	    void removeNode(NodeId aNodeId);

	    const std::set<NodeId> & getNodes() const;

	    bool contains(NodeId aNodeId) const;
	    
	private:
	    std::set<NodeId> mNodes;
	};
	
	/**
	 * A graph in NodeJS is the unit of computation much like a
	 * function is the unit of computation in a procedural
	 * programming language.  Like methods, graphs consist
	 * of a set of computations linked together in an order.
	 * In a NodeJS program, the order is expressed by the
	 * graph structure of edges connecting computations together
	 * and the computations themselves are represented by nodes.
	 *
	 * Also, like functions or methods, there is a notion of scope.
	 * A scope is simply a local namespace where other objects
	 * can be referenced.  In the case of NodeJS, the objects
	 * in the scope are node types.  We express by a scope
	 * the set of node types that are visible from within this
	 * graph.  This allows construction of large programs
	 * where name collisions are possible globally, each local
	 * scope is declared in a way that limits the risk of
	 * name collisions.
	 */
	class NodeGraph {
	public:
	    NodeGraph(NodeModule & aModule);
	    ~NodeGraph();

	    NodeModule & getModule() const;
	    
	    /**
	     * Creates a new node of the given type, attempting to use
	     * the given string as the node ID.  If this
	     * would violate the node's uniqueness, an
	     * identifier is added in order to disambiguate
	     * and ensure each node gets a unique id.
	     *
	     * The node's internal data is initialized with
	     * the given data.  This is mainly used for loading
	     * nodes from somewhere else.
	     */
	    Node & newNode(
		const NodeType & aNodeType,
		const std::string & aNodeIdCandidate,
		const ConnectionData & aConnectionData
		);

	    /**
	     * If we don't specify connection data for the node,
	     * it will pull the default data from the node type.
	     * This is mainly used for creating new nodes.
	     */
	    Node & newNode(
		const NodeType & aNodeType,
		const std::string & aNodeIdCandidate
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
	     * Removed a node from the graph along with any associated edges.
	     */
	    void removeNode(NodeId aNodeId);
	    
	    /**
	     * Returns a map of nodes
	     */
	    const std::map<NodeId, std::unique_ptr<Node>> & getNodes() const;
	    
	    /**
	     * Attempts to create a new connection between
	     * the given node and ports.  If the connection
	     * already existed, we just return that.  If not,
	     * a new connection id is created and returned.
	     * If one of the nodes or ports does not exist,
	     * then the optional is returned without a
	     * connection id indicating that it could not be created.
	     */
	    std::optional<EdgeId> newEdge(
		NodeId aFromNode,
		PortId aFromPort,
		NodeId aToNode,
		PortId aToPort
		);

	    void removeEdge(
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
	    const std::vector<const Edge *> & getEdgesFrom(NodeId aNodeId) const;

	    /**
	     * Return a list of the connections terminating
	     * at the given node id.  That is, all connections
	     * with a 'to node' equal to the given node.
	     */
	    const std::vector<const Edge*> & getEdgesTo(NodeId aNodeId) const;

	    const std::map<EdgeId, std::unique_ptr<Edge>> & getEdges() const;

	    const std::vector<const NodeModule *> & getScopes() const;

	    void addScope(const NodeModule * node_module);

	    void copyScope(const NodeGraph *other);
	    
	    const NodeType * getNodeType(std::string aTypeName);

	    std::set<NodeId> getNodeIds(void) const;

	    std::set<NodeId> getNeighborNodeIds(NodeId aNode) const;
            /**
	     * This method sorts the nodes in topological
	     * order and returns the associated NodeIds
	     * This method will return no value (in the optional)
	     * if the graph has a cycle.
	     */
	    std::optional<std::vector<NodeId>> getNodeIdsInTopologicalOrder(void) const;

	    /**
	     * Node groups are simply named sets of nodes.
	     * A node may belong to zero or more groups.
	     * Groups may have zero or more nodes.
	     * Node grouping is a useful meta-programming
	     * construct as well as an organizational tool
	     * for viewing and thinking about nodes and their
	     * purposes.
	     */
	    const std::map<GroupId, std::unique_ptr<Group>> & getGroups() const;

	    void addGroup(GroupId aGroupId);

	    void removeGroup(GroupId aGroupId);

	    Group *getGroup(GroupId aGroupId) const;
	    
	    /**
	     * Metadata is data that can be associated
	     * with a node module that can be used by other
	     * extensions to the system.
	     */
	    const Metadata & getMetadata(void) const;
	    Metadata & getMetadata(void);

	private:
	    NodeModule & mModule;

	    std::string findNewNodeId(std::string aNodeIdCandidate);
	    
	    // The core of a graph is the nodes and edges.
	    std::map<NodeId, std::unique_ptr<Node>> mNodes;
	    std::map<GroupId, std::unique_ptr<Group>> mGroups;
	    
	    std::map<EdgeId, std::unique_ptr<Edge>> mEdges;
	    std::map<NodeId, std::vector<const Edge*>> mEdgesByFromNode;
	    std::map<NodeId, std::vector<const Edge*>> mEdgesByToNode;

	    std::vector<const NodeModule*> mScopes;
	    
	    Metadata mMetadata;
	};
    }
}
