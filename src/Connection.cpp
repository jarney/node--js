#include "node--js/Connection.hpp"

using namespace NodeJS::core;

Connection::Connection(
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
Connection::operator==(const Connection & other) const
{
    return (this->fromNode == other.fromNode) &&
	(this->fromPort == other.fromPort) &&
	(this->toNode == other.toNode) &&
	(this->toPort == other.toPort);
}

bool
Connection::operator!=(const Connection & other) const
{
    return !(*this == other);
}
ConnectionId
Connection::getId() const
{
    return fromNode + std::string("-") + fromPort +
	std::string("|") + 
	toNode + std::string("-") + toPort;
}
