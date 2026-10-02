#include "node--js/NodeGraph.hpp"

using namespace NodeJS::core;

NodeGraph::NodeGraph()
{}

NodeGraph::~NodeGraph()
{}

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

std::optional<ConnectionId>
NodeGraph::newEdge(
    NodeId aFromNode,
    PortId aFromPort,
    NodeId aToNode,
    PortId aToPort
    )
{
    std::unique_ptr<Connection> connection = std::make_unique<Connection>(
	aFromNode, aFromPort,
	aToNode, aToPort
	);
    std::string id = connection->getId();
    if (mEdges.find(id) != mEdges.end()) {
	return std::optional<ConnectionId>();
    }

    mEdgesByFromNode[aFromNode].push_back(connection.get());
    mEdgesByToNode[aToNode].push_back(connection.get());
    mEdges.insert(std::pair(id, std::move(connection)));
    return id;
}

const std::map<ConnectionId, std::unique_ptr<Connection>> &
NodeGraph::getEdges() const
{
    return mEdges;
}


