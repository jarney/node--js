#include "node--js/NodeModule.hpp"

using namespace NodeJS::core;

NodeModule::NodeModule(ModuleLoader & aModuleLoader)
    : mModuleLoader(aModuleLoader)
{}

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

ModuleLoader &
NodeModule::getModuleLoader(void) const
{
    return mModuleLoader;
}
	    

void
NodeModule::addDataType(std::unique_ptr<DataType> dataType)
{
    mDataTypes.insert(std::make_pair(dataType->getId(), std::move(dataType)));
}

void
NodeModule::removeDataType(std::string name)
{
    mDataTypes.erase(name);
}

const
std::map<std::string, std::unique_ptr<DataType>> &
NodeModule::getDataTypes() const
{
    return mDataTypes;
}

bool
NodeModule::hasDataType(const std::string & name) const
{
    return mDataTypes.count(name) != 0;
}

bool
NodeModule::hasNodeType(const std::string & name) const
{
    return mNodeTypes.count(name) != 0;
}

const DataType *
NodeModule::getDataType(const std::string & name) const
{
    const auto & it = mDataTypes.find(name);
    if (it == mDataTypes.end()) {
	return nullptr;
    }
    return it->second.get();
}

/////////////////////////////////////
void
NodeModule::addNodeType(std::unique_ptr<NodeType> nodeType)
{
    mNodeTypes.insert(std::make_pair(nodeType->getId(), std::move(nodeType)));
}

void
NodeModule::removeNodeType(std::string name)
{
    mNodeTypes.erase(name);
}

const NodeType *
NodeModule::getNodeType(const std::string & name) const
{
    const auto & it = mNodeTypes.find(name);
    if (it == mNodeTypes.end()) {
	return nullptr;
    }
    return it->second.get();
}

const
std::map<std::string, std::unique_ptr<NodeType>> &
NodeModule::getNodeTypes() const
{
    return mNodeTypes;
}

NodeGraph *
NodeModule::addGraph(std::string id)
{
    if (mGraphs.find(id) != mGraphs.end()) {
	return nullptr;
    }
    std::unique_ptr<NodeGraph> graph = std::make_unique<NodeGraph>(*this);
    NodeGraph *ret = graph.get();
    mGraphs.insert(std::pair(id, std::move(graph)));
    return ret;
}

NodeGraph *
NodeModule::getGraph(std::string id)
{
    const auto & it = mGraphs.find(id);
    if (it == mGraphs.end()) {
	return nullptr;
    }
    return it->second.get();
}

const std::map<std::string, std::unique_ptr<NodeGraph>> &
NodeModule::getGraphs() const
{
    return mGraphs;
}

