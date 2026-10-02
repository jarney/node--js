#include "node--js/NodeGraph.hpp"
#include "node--js/NodeModule.hpp"

using namespace NodeJS::core;

NodeGraph::NodeGraph(NodeModule & aModule)
    : mModule(aModule)
{}

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

const std::map<EdgeId, std::unique_ptr<Edge>> &
NodeGraph::getEdges() const
{
    return mEdges;
}


void
NodeGraph::addScope(std::unique_ptr<NodeModule> node_module)
{
    mScopes.push_back(std::move(node_module));
}

const NodeType *
NodeGraph::getNodeType(std::string aTypeName)
{
    for (const auto & it : mScopes) {
	if (it->hasNodeType(aTypeName)) {
	    return it->getNodeType(aTypeName);
	}
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
