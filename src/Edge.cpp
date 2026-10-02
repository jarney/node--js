#include "node--js/Edge.hpp"

using namespace NodeJS::core;

Edge::Edge(
    NodeId aFromNode,
    PortId aFromPort,
    NodeId aToNode,
    PortId aToPort
    )
    : fromNode(aFromNode)
    , fromPort(aFromPort)
    , toNode(aToNode)
    , toPort(aToPort)
{}

bool
Edge::operator==(const Edge & other) const
{
    return (this->fromNode == other.fromNode) &&
	(this->fromPort == other.fromPort) &&
	(this->toNode == other.toNode) &&
	(this->toPort == other.toPort);
}

bool
Edge::operator!=(const Edge & other) const
{
    return !(*this == other);
}
EdgeId
Edge::getId() const
{
    return fromNode + std::string("-") + fromPort +
	std::string("|") + 
	toNode + std::string("-") + toPort;
}
