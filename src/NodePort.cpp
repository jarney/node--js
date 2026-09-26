#include "node--js/NodePort.hpp"

using namespace NodeJS::core;

NodePort::NodePort(
    const NodeDataType & type,
    NodePort::ConnectionPolicy policy
    )
    : mType(type)
    , mConnectionPolicy(policy)
{}

NodePort::NodePort(const NodeDataType & type)
    : NodePort(type, NodePort::ConnectionPolicy::One)
{}

const NodeDataType &
NodePort::getDataType() const
{ return mType; }

const NodePort::ConnectionPolicy & 
NodePort::getConnectionPolicy() const
{ return mConnectionPolicy; }

