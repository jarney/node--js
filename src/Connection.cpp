#include "node--js/Connection.hpp"

using namespace NodeJS::core;

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
