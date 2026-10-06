#pragma once

#include "node--js/NodePort.hpp"
#include <string>
#include <vector>
#include <map>
#include <memory>

namespace NodeJS {
    namespace core {

	class NamedPorts {
	public:
	    NamedPorts() = default;
	    ~NamedPorts() = default;

	    bool addPort(std::string name, std::unique_ptr<NodePort> port);
	    const NodePort *getByName(std::string name) const;
	    const NodePort *getByIndex(unsigned int index) const;
	    bool hasPort(std::string name) const;
	    std::string getName(unsigned int index) const;
	    size_t getPortIndex(std::string name) const;
	    size_t getCount() const;

	private:
	    std::map<std::string, std::unique_ptr<NodePort>> mPortsByName;
	    std::vector<NodePort*> mPorts;
	    std::vector<std::string> mPortNames;
	};
	
    }
}

	
