#include "node--js/NamedPorts.hpp"

#include <limits.h>

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

size_t
NamedPorts::getPortIndex(std::string name) const
{
    size_t i = 0;
    for (i = 0; i < mPortNames.size(); i++) {
	if (name == mPortNames.at(i)) {
	    return i;
	}
    }
    return INT_MAX;
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

size_t
NamedPorts::getCount() const
{
    return mPorts.size();
}

