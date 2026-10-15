#include "node--js/NodeModule.hpp"

using namespace NodeJS::core;

const std::string &
NodeType::getLocalId() const
{
    return mId;
}

const std::string &
NodeType::getFullyQualifiedId() const
{
    return mFullyQualifiedId;
}

void
NodeType::setId(std::string id, std::string package)
{
    mId = id;
    mFullyQualifiedId = package + std::string(".") + id;
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

const Metadata &
NodeType::getMetadata(void) const
{
    return mMetadata;
}

Metadata &
NodeType::getMetadata(void)
{
    return mMetadata;
}
