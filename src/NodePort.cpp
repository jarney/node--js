#include "node--js/NodePort.hpp"

using namespace NodeJS::core;

NodePort::NodePort(
    const DataType & type,
    NodePort::ConnectionPolicy policy
    )
    : mType(type)
    , mConnectionPolicy(policy)
{}

NodePort::NodePort(const DataType & type)
    : NodePort(type, NodePort::ConnectionPolicy::One)
{}

const DataType &
NodePort::getDataType() const
{ return mType; }

const NodePort::ConnectionPolicy & 
NodePort::getConnectionPolicy() const
{ return mConnectionPolicy; }

