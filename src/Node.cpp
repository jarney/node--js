#include "node--js/Node.hpp"

using namespace NodeJS::core;

Node::Node(
    NodeId aId,
    const NodeType & aType,
    NodeGraph & aGraph,
    const ConnectionData & aData
    )
    : mId(aId)
    , mType(aType)
    , mGraph(aGraph)
    , mData(aData)
    , mOverrideInputs(false)
    , mOverrideOutputs(false)
    , mInputs()
    , mOutputs()
{}

const NodeId &
Node::getId() const
{ return mId; }

const NodeType &
Node::getType() const
{ return mType; }

NodeGraph &
Node::getGraph() const
{ return mGraph; }

ConnectionData &
Node::getData()
{ return mData; }

const ConnectionData &
Node::getData() const
{ return mData; }

const NamedPorts &
Node::getInputs() const
{
    if (mOverrideInputs) return mInputs;
    return mType.getInputs();
}

const NamedPorts &
Node::getOutputs() const
{
    if (mOverrideOutputs) return mOutputs;
    return mType.getOutputs();
}

void
Node::setOverrideInputs(bool aOverrideInputs)
{
    mOverrideInputs = aOverrideInputs;
}
void
Node::setOverrideOutputs(bool aOverrideOutputs)
{
    mOverrideOutputs = aOverrideOutputs;
}

NamedPorts &
Node::getOverrideInputs()
{
    return mInputs;
}

NamedPorts &
Node::getOverrideOutputs()
{
    return mOutputs;
}

const NamedPorts &
Node::getOverrideInputs() const
{
    return mInputs;
}

const NamedPorts &
Node::getOverrideOutputs() const
{
    return mOutputs;
}

bool
Node::hasOverrideInputs() const
{
    return mOverrideInputs;
}
bool
Node::hasOverrideOutputs() const
{
    return mOverrideOutputs;
}

const Metadata &
Node::getMetadata(void) const
{
    return mMetadata;
}

Metadata &
Node::getMetadata(void)
{
    return mMetadata;
}
