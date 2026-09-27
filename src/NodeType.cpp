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

NodeJS::core::NodeType::Implementation
NodeType::getImplementation(void) const
{
    return mImplementation;
}

void
NodeType::setImplementation(NodeJS::core::NodeType::Implementation impl)
{
    mImplementation = impl;
}

