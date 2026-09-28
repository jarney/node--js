#include "node--js/NodePort.hpp"

using namespace NodeJS::core;

NodePort::NodePort(
    std::string type,
    std::string description,
    NodePort::ConnectionPolicy policy
    )
    : mType(type)
    , mDescription(description)
    , mConnectionPolicy(policy)
{}

NodePort::NodePort(
    std::string type,
    std::string description
    )
    : NodePort(
	type,
	description,
	NodePort::ConnectionPolicy::One
	)
{}

const std::string &
NodePort::getDataType() const
{ return mType; }

const std::string &
NodePort::getDescription() const
{ return mDescription; }

const NodePort::ConnectionPolicy & 
NodePort::getConnectionPolicy() const
{ return mConnectionPolicy; }

