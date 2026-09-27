#pragma once

#include <string>
#include <map>
#include <memory>

#include "node--js/DataType.hpp"
#include "node--js/NodeType.hpp"

namespace NodeJS {
    namespace core {
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
	    NodeModule() = default;
	    ~NodeModule() = default;

	    /**
	     * Sets the fully-qualified package name.
	     */
	    void setPackage(std::string package);
	    
	    /**
	     * Returns the fully-qualified package name.
	     */
	    std::string getPackage(void) const;
	    
	    /**
	     * This method adds a data type to the module.
	     */
	    void addDataType(DataType dataType);
	    /**
	     * This method removes a data type from a module.
	     * This will refuse to act if the data type is still
	     * in use anywhere in the module.
	     */
	    void removeDataType(std::string name);

	    /**
	     * This returns a map of data types in the module.
	     */
	    const std::map<std::string, DataType> & getDataTypes() const;

	    /**
	     * Returns true if the given data type is registered in this module.
	     */
	    bool hasDataType(std::string name) const;

	    /**
	     * C++ does not provide an optional reference
	     * type, so the closest we can get is a pointer.
	     * This returns the data type if it exists and nullptr
	     * if it does not.
	     */
	    const DataType *getDataType(std::string & name) const;

	    const std::map<std::string, std::unique_ptr<NodeType>> & getNodeTypes() const;

	    void addNodeType(std::string id, std::unique_ptr<NodeType> nodeType);
	    
	private:
	    std::string mPackage;
	    std::map<std::string, DataType> mDataTypes;
	    std::map<std::string, std::unique_ptr<NodeType>> mNodeTypes;
	};
    }
}
