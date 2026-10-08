#include "node--js/Group.hpp"
#include "node--js/NodeGraph.hpp"

using namespace NodeJS::core;

void
Group::addNode(NodeId aNodeId)
{
    mNodes.insert(aNodeId);
}

void
Group::removeNode(NodeId aNodeId)
{
    mNodes.erase(aNodeId);
}


const std::set<NodeId> &
Group::getNodes() const
{
    return mNodes;
}


bool
Group::contains(NodeId aNodeId) const
{
    return (mNodes.find(aNodeId) != mNodes.end());
}

