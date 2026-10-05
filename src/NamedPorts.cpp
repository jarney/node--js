#include "node--js/NamedPorts.hpp"

using namespace NodeJS::core;

bool
NamedPorts::addPort(std::string name, std::unique_ptr<NodePort> port)
{
    if (mPortsByName.count(name) != 0) return false;

    mPorts.push_back(port.get());
    mPortNames.push_back(name);
    mPortsByName.insert(std::make_pair(name, std::move(port)));
    return true;
}

const NodePort *
NamedPorts::getByName(std::string name) const
{
    const auto & it = mPortsByName.find(name);
    if (it == mPortsByName.end()) {
	return nullptr;
    }
    return it->second.get();
}

const NodePort *
NamedPorts::getByIndex(unsigned int index) const
{
    if (index >= mPorts.size()) {
	return nullptr;
    }
    NodePort *port = mPorts.at(index);
    return port;
}

bool
NamedPorts::hasPort(std::string name) const
{
    return mPortsByName.count(name) != 0;
}

std::string
NamedPorts::getName(unsigned int index) const
{
    if (index >= mPortNames.size()) {
	return std::string();
    }
    std::string portName = mPortNames.at(index);
    return portName;
}

int
NamedPorts::getCount() const
{
    return mPorts.size();
}

