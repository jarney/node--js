#pragma once

#include <string>
#include <map>
#include <memory>

#include "node--js/DataType.hpp"
#include "node--js/NodeType.hpp"
#include "node--js/NodeGraph.hpp"

namespace NodeJS {
    namespace core {
	class ModuleLoader;

	typedef std::string GraphId;
	
	/**
	 * A NodeJS module consists of a collection of data types
	 * and node types.  These represent the possible types available
	 * to construct graphs from.  Data types are completely abstract
	 * in nature and it is up to the unerlying nodes to encode and
	 * decode the data traveling across the connections.
	 *
	 * The node types themselves are implemented either as 'native'
	 * implementations or as graphs made up of other nodes.
	 * The 'native' implementations are provided by the runtime
	 * (C++ or other implementation provided as a library).
	 * The 'graph' implementations are provided by a directed
	 * acyclic graph of nodes connected together to compute the result.
	 */
	class NodeModule {
	public:
	    NodeModule(ModuleLoader & moduleLoader);
	    ~NodeModule() = default;

	    /**
	     * Sets the fully-qualified package name.
	     */
	    void setPackage(std::string aPackage);
	    
	    /**
	     * Returns the fully-qualified package name.
	     */
	    std::string getPackage(void) const;

	    void setDescription(std::string aDescription);
	    
	    std::string getDescription(void) const;
	    
	    ModuleLoader & getModuleLoader(void) const;
	    
	    /**
	     * This method adds a data type to the module.
	     */
	    void addDataType(std::unique_ptr<DataType> dataType);
	    /**
	     * This method removes a data type from a module.
	     * This will refuse to act if the data type is still
	     * in use anywhere in the module.
	     */
	    void removeDataType(std::string name);

	    /**
	     * This returns a map of data types in the module.
	     */
	    const std::map<std::string, std::unique_ptr<DataType>> & getDataTypes() const;

	    /**
	     * Returns true if the given data type is registered in this module.
	     */
	    bool hasDataType(const std::string & name) const;

	    /**
	     * C++ does not provide an optional reference
	     * type, so the closest we can get is a pointer.
	     * This returns the data type if it exists and nullptr
	     * if it does not.
	     */
	    const DataType *getDataType(const std::string & name) const;

	    void addNodeType(std::unique_ptr<NodeType> nodeType);

	    void removeNodeType(std::string name);

	    const NodeType *getNodeType(const std::string & name) const;

	    const std::map<std::string, std::unique_ptr<NodeType>> & getNodeTypes() const;

	    bool hasNodeType(const std::string & name) const;

	    NodeGraph *addGraph(std::string id);

	    NodeGraph *getGraph(std::string id);

	    const std::map<std::string, std::unique_ptr<NodeGraph>> & getGraphs() const;

	private:
	    ModuleLoader & mModuleLoader;
	    std::string mPackage;
	    std::string mDescription;
	    std::map<std::string, std::unique_ptr<DataType>> mDataTypes;
	    std::map<std::string, std::unique_ptr<NodeType>> mNodeTypes;
	    std::map<std::string, std::unique_ptr<NodeGraph>> mGraphs;
	};
    }
}
