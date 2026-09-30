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
