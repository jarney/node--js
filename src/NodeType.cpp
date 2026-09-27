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


bool
NodeType::addInputPort(std::string name, std::unique_ptr<NodePort> port)
{
    if (mInputsByName.count(name) != 0) return false;

    mInputs.push_back(port.get());
    mInputNames.push_back(name);
    mInputsByName.insert(std::make_pair(name, std::move(port)));
    return true;
}

const NodePort *
NodeType::getInputPortByName(std::string name) const
{
    const auto & it = mInputsByName.find(name);
    if (it == mInputsByName.end()) {
	return nullptr;
    }
    return it->second.get();
}

bool
NodeType::hasInputPort(std::string name) const
{
    return mInputsByName.count(name) != 0;
}

const NodePort *
NodeType::getInputPortByIndex(int index) const
{
    if (index >= mInputs.size()) {
	return nullptr;
    }
    NodePort *port = mInputs.at(index);
    return port;
}

std::string
NodeType::getInputPortName(int index) const
{
    if (index >= mInputNames.size()) {
	return std::string();
    }
    std::string portName = mInputNames.at(index);
    return portName;
}

int
NodeType::getInputPortCount(void) const
{
    return mInputs.size();
}

bool
NodeType::addOutputPort(std::string name, std::unique_ptr<NodePort> port)
{
    if (mOutputsByName.count(name) != 0) return false;

    mOutputs.push_back(port.get());
    mOutputNames.push_back(name);
    mOutputsByName.insert(std::make_pair(name, std::move(port)));
    return true;
}

const NodePort *
NodeType::getOutputPortByName(std::string name) const
{
    const auto & it = mOutputsByName.find(name);
    if (it == mOutputsByName.end()) {
	return nullptr;
    }
    return it->second.get();
}

bool
NodeType::hasOutputPort(std::string name) const
{
    return mOutputsByName.count(name) != 0;
}

const NodePort *
NodeType::getOutputPortByIndex(int index) const
{
    if (index >= mOutputs.size()) {
	return nullptr;
    }
    NodePort *port = mOutputs.at(index);
    return port;
}

std::string
NodeType::getOutputPortName(int index) const
{
    if (index >= mOutputNames.size()) {
	return std::string();
    }
    std::string portName = mOutputNames.at(index);
    return portName;
}

int
NodeType::getOutputPortCount(void) const
{
    return mOutputs.size();
}
