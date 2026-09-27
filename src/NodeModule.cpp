#include "node--js/NodeModule.hpp"

using namespace NodeJS::core;

void
NodeModule::setPackage(std::string package)
{
    mPackage = package;
}

std::string
NodeModule::getPackage(void) const
{
    return mPackage;
}
	    

void
NodeModule::addDataType(DataType dataType)
{
    mDataTypes.insert(std::make_pair(dataType.getId(), dataType));
}

void
NodeModule::removeDataType(std::string name)
{
    mDataTypes.erase(name);
}

const
std::map<std::string, DataType> &
NodeModule::getDataTypes() const
{
    return mDataTypes;
}

bool
NodeModule::hasDataType(std::string name) const
{
    return mDataTypes.count(name) != 0;
}

const DataType *
NodeModule::getDataType(std::string & name) const
{
    const auto & it = mDataTypes.find(name);
    if (it == mDataTypes.end()) {
	return nullptr;
    }
    return &it->second;
}
/////////////////////////////////////

const
std::map<std::string, std::unique_ptr<NodeType>> &
NodeModule::getNodeTypes() const
{
    return mNodeTypes;
}
void
NodeModule::addNodeType(std::string id, std::unique_ptr<NodeType> nodeType)
{
    mNodeTypes.insert(std::make_pair(id, std::move(nodeType)));
}

