#include "node--js/NodePort.hpp"

using namespace NodeJS::core;

NodePort::NodePort(
    std::string aDataType,
    std::string aDescription,
    NodePort::ConnectionPolicy aPolicy
    )
    : mDataType(aDataType)
    , mDescription(aDescription)
    , mConnectionPolicy(aPolicy)
{}

NodePort::NodePort(
    std::string aDataType,
    std::string aDescription
    )
    : NodePort(
	aDataType,
	aDescription,
	NodePort::ConnectionPolicy::One
	)
{}

const std::string &
NodePort::getDataType() const
{ return mDataType; }

const std::string &
NodePort::getDescription() const
{ return mDescription; }

const NodePort::ConnectionPolicy & 
NodePort::getConnectionPolicy() const
{ return mConnectionPolicy; }

const Metadata &
NodePort::getMetadata(void) const
{ return mMetadata; }

Metadata &
NodePort::getMetadata(void)
{ return mMetadata; }
