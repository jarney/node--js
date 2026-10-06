#include "node--js/NodeModule.hpp"

using namespace NodeJS::core;

const std::string &
NodeType::getId() const
{
    return mId;
}

void
NodeType::setId(std::string id)
{
    mId = id;
}

NodeJS::core::NodeType::Visibility
NodeType::getVisibility(void) const
{
    return mVisibility;
}

void
NodeType::setVisibility(NodeJS::core::NodeType::Visibility visibility)
{
    mVisibility = visibility;
}

void
NodeType::setAllowPortOverride(bool aAllowPortOverride)
{
    mAllowPortOverride = aAllowPortOverride;
}
bool
NodeType::getAllowPortOverride(void) const
{
    return mAllowPortOverride;
}

NodeJS::core::NodeType::Type
NodeType::getType(void) const
{
    return mType;
}

void
NodeType::setType(NodeJS::core::NodeType::Type type)
{
    mType = type;
}


const NamedPorts &
NodeType::getInputs() const
{ return mInputs; }

const NamedPorts &
NodeType::getOutputs() const
{ return mOutputs; }

NamedPorts &
NodeType::getInputs()
{ return mInputs; }

NamedPorts &
NodeType::getOutputs()
{ return mOutputs; }

const ConnectionData &
NodeType::getDefaultNodeData() const
{
    return mDefaultNodeData;
}

void
NodeType::setDefaultNodeData(const ConnectionData & aDefaultNodeData)
{
    mDefaultNodeData = aDefaultNodeData;
}
