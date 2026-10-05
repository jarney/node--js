#include "node--js/NodeGraph.hpp"
#include "node--js/NodeModule.hpp"

#include <algorithm>

using namespace NodeJS::core;

NodeGraph::NodeGraph(NodeModule & aModule)
    : mModule(aModule)
{
    // Is this always what we want?
    // Is the current module always in scope?
    mScopes.push_back(&mModule);
}

NodeGraph::~NodeGraph()
{}

NodeModule &
NodeGraph::getModule() const
{
    return mModule;
}

std::string
NodeGraph::findNewNodeId(std::string aNodeIdCandidate)
{
    std::string actualNewId = aNodeIdCandidate;
    int i = 0;
    while (mNodes.find(actualNewId) != mNodes.end()) {
	actualNewId = aNodeIdCandidate + "-" + std::to_string(i);
	i++;
    }
    return actualNewId;
}


Node &
NodeGraph::newNode(
    const NodeType & aNodeType,
    const std::string & aNodeIdCandidate,
    const ConnectionData & aConnectionData
    )
{
    std::string newNodeId = findNewNodeId(aNodeIdCandidate);

    std::unique_ptr<Node> newNode = std::make_unique<Node>(newNodeId, aNodeType, *this, aConnectionData);
    Node & retNode = *newNode;
    mNodes.insert(std::pair(newNodeId, std::move(newNode)));
    return retNode;
}


Node *
NodeGraph::getNode(NodeId aNodeId) const
{
    const auto & it = mNodes.find(aNodeId);
    if (it == mNodes.end()) {
	return nullptr;
    }
    return it->second.get();
}


bool
NodeGraph::hasNode(NodeId aNodeId) const
{
    const auto & it = mNodes.find(aNodeId);
    if (it == mNodes.end()) {
	return false;
    }
    return true;
}

/**
 * Returns a map of nodes
 */
const std::map<NodeId, std::unique_ptr<Node>> &
NodeGraph::getNodes() const
{
    return mNodes;
}

std::optional<EdgeId>
NodeGraph::newEdge(
    NodeId aFromNode,
    PortId aFromPort,
    NodeId aToNode,
    PortId aToPort
    )
{
    std::unique_ptr<Edge> connection = std::make_unique<Edge>(
	aFromNode, aFromPort,
	aToNode, aToPort
	);
    std::string id = connection->getId();
    if (mEdges.find(id) != mEdges.end()) {
	return std::optional<EdgeId>();
    }

    mEdgesByFromNode[aFromNode].push_back(connection.get());
    mEdgesByToNode[aToNode].push_back(connection.get());
    mEdges.insert(std::pair(id, std::move(connection)));
    return id;
}

static const std::vector<const Edge *> empty_edges;

const std::vector<const Edge *> &
NodeGraph::getEdgesFrom(NodeId aNodeId) const
{
    const auto edgesIt = mEdgesByFromNode.find(aNodeId);
    if (edgesIt == mEdgesByFromNode.end()) return empty_edges;
    return edgesIt->second;
}

const std::vector<const Edge *> &
NodeGraph::getEdgesTo(NodeId aNodeId) const
{
    const auto edgesIt = mEdgesByToNode.find(aNodeId);
    if (edgesIt == mEdgesByToNode.end()) return empty_edges;
    return edgesIt->second;
}

const std::map<EdgeId, std::unique_ptr<Edge>> &
NodeGraph::getEdges() const
{
    return mEdges;
}

void
NodeGraph::addScope(const NodeModule *node_module)
{
    mScopes.push_back(node_module);
}

void
NodeGraph::copyScope(const NodeGraph *other)
{
    mScopes = other->mScopes;
}

const std::vector<const NodeModule *> &
NodeGraph::getScopes() const
{ return mScopes; }

const NodeType *
NodeGraph::getNodeType(std::string aTypeName)
{
    for (const auto & it : mScopes) {
	if (aTypeName == "a") {
	    fprintf(stderr, "Searching for node type %s in module %p\n",
		    aTypeName.c_str(), it);
	}
	if (it->hasNodeType(aTypeName)) {
	    return it->getNodeType(aTypeName);
	}
    }
    if (mScopes.size() == 0) {
	fprintf(stderr, "This graph doesn't have any scopes?!\n");
    }
    return nullptr;
}

#if 0
NodeId
NodeGraph::addNode(std::string aNodeTypeId)
{
    const NodeType *nodeType = getNodeType(aNodeTypeId);
    if (nodeType == nullptr) {
	fprintf(stderr, "Error loading node of type %s\n", aNodeTypeId.c_str());
	return "";
    }
    ConnectionData defaultData;
    Node & n = newNode(
	*nodeType,
	aNodeTypeId,
	defaultData
	);
    return n.getId();
}
#endif

std::set<NodeId>
NodeGraph::getNodeIds(void) const
{
    std::set<NodeId> nodes;
    for (const auto & it : mNodes) {
	nodes.insert(it.first);
    }
    return nodes;
}


std::set<NodeId>
NodeGraph::getNeighborNodeIds(NodeId aNode) const
{
    std::set<NodeId> neighbors;

    // No edges means empty neighbor graph
    const auto & edges = mEdgesByFromNode.find(aNode);
    if (edges == mEdgesByFromNode.end()) {
	return neighbors;
    }
    for (const auto & edge : edges->second) {
	neighbors.insert(edge->toNode);
    }

    return neighbors;
}

// Useful for states in the
// topological sort algorithm used
// to unwind the dependency order.
static constexpr int TOPOSORT_STATE_TODO = 0;
static constexpr int TOPOSORT_STATE_IN_PROGRESS = 1;
static constexpr int TOPOSORT_STATE_PROCESSED = 2;

std::optional<std::vector<NodeId>>
NodeGraph::getNodeIdsInTopologicalOrder(void) const
{
    std::map<NodeId, int> state;
    for (const auto & nodeId : getNodeIds()) {
	state[nodeId] = TOPOSORT_STATE_TODO;
    }
    std::vector<NodeId> order;

    for (const auto & start : getNodeIds()) {
	if (state[start] != TOPOSORT_STATE_TODO) {
	    continue;
	}
	std::vector<std::pair<NodeId, bool>> stack;
	stack.push_back(std::make_pair(start, false));

	while (stack.size() > 0) {
	    std::pair<NodeId, bool> pair = stack.back();
	    stack.pop_back();

	    NodeId node = pair.first;
	    bool processed = pair.second;
	    
	    if (processed) {
		state[node] = TOPOSORT_STATE_PROCESSED;
		order.push_back(node);
		continue;
	    }
	    if (state[node] == TOPOSORT_STATE_PROCESSED) {
		continue;
	    }
	    if (state[node] == TOPOSORT_STATE_IN_PROGRESS) {
		return std::optional<std::vector<NodeId>>();
	    }
	    state[node] = TOPOSORT_STATE_IN_PROGRESS;
	    stack.push_back(std::make_pair(node, true));

	    for ( const auto & neighbor : getNeighborNodeIds(node)) {
		if (state[neighbor] == TOPOSORT_STATE_IN_PROGRESS) {
		    return std::optional<std::vector<NodeId>>();
		}
		if (state[neighbor] == TOPOSORT_STATE_TODO) {
		    stack.push_back(std::make_pair(neighbor, false));
		}
	    }
	}
    }
    std::reverse(order.begin(), order.end());
    return std::optional(order);
}

