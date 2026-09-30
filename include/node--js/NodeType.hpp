#pragma once

#include <string>
#include <map>
#include <vector>
#include <memory>

#include "node--js/NodePort.hpp"

namespace NodeJS {
    namespace core {
	/**
	 * This class represents a type of node in a node program.
	 * Node types are characterized by a unique identifier.
	 * Node types carry a visibility field indicating whether
	 * nodes outside this package are allowed to access it or not.
	 * In addition, a node may be marked as 'native', indicating that
	 * its implementation is provided by the underlying
	 * runtime, or as 'graph' indicating that the implementation
	 * is provided by a graph of other nodes.
	 *
	 * In addition, each node declares input and output ports
	 * which may carry data.  Each input and output port are
	 * associated with a data type.
	 */
	typedef std::string PortId;
	
	class NodeType {
	public:

	    /**
	     * This enum represents the visibility of a node type.
	     * This determines whether graphs outside this package are
	     * permitted to access it.
	     */
	    typedef enum {
		/**
		 * This indicates that a node type is public and may
		 * be used in any graph context.
		 */
		PUBLIC,
		/**
		 * This indicates that a node type is private
		 * and may be used only in the context of the node module
		 * where it was declared.
		 */
		PRIVATE
	    } Visibility;

	    /**
	     * This enum represents the declaration of how a node
	     * is implemented.
	     */
	    typedef enum {
		/**
		 * This indicates that the node's implementation is
		 * provided by the underlying runtime.
		 */
		NATIVE,
		/**
		 * This indicates that the node's implementation is
		 * provided by a node graph consisting of other node types.
		 */
		GRAPH
	    } Type;
	    
	    NodeType() = default;
	    ~NodeType() = default;

	    const PortId & getId() const;
	    void setId(PortId id);
	    
	    
	    /**
	     * Returns the visibility of this node type.
	     * This determines whether the node type is
	     * visible outside the scope of this module
	     * or not.  This is useful for organizing
	     * code into modules where part of the
	     * impelementation is hidden.
	     */
	    NodeJS::core::NodeType::Visibility getVisibility(void) const;

	    void setVisibility(NodeJS::core::NodeType::Visibility visibility);

	    /**
	     * This returns the implementation type
	     * for this type.  Some types may be
	     * implemented on the underlying runtime
	     * rather than being implemented in terms
	     * of other nodes as a graph.
	     */
	    NodeJS::core::NodeType::Type getType(void) const;

	    /**
	     * This sets the implementation as either graph
	     * or native.
	     */
	    void setType(NodeJS::core::NodeType::Type impl);

	    /**
	     * Adds a new port to the node type.
	     * This returns false if the node already existed.
	     */
	    bool addInputPort(std::string name, std::unique_ptr<NodePort> port);

	    const NodePort *getInputPortByName(std::string name) const;
	    const NodePort *getInputPortByIndex(unsigned int index) const;
	    bool hasInputPort(std::string name) const;
	    std::string getInputPortName(unsigned int index) const;
	    int getInputPortCount() const;

	    /**
	     * Adds a new port to the node type.
	     * This returns false if the node already existed.
	     */
	    bool addOutputPort(std::string name, std::unique_ptr<NodePort> port);

	    const NodePort *getOutputPortByName(std::string name) const;
	    const NodePort *getOutputPortByIndex(unsigned int index) const;
	    bool hasOutputPort(std::string name) const;
	    std::string getOutputPortName(unsigned int index) const;
	    int getOutputPortCount() const;
	    
	private:
	    std::string mId;
	    Visibility mVisibility;
	    Type mType;

	    // Ports by name
	    std::map<std::string, std::unique_ptr<NodePort>> mInputsByName;
	    std::map<std::string, std::unique_ptr<NodePort>> mOutputsByName;

	    // Ports by index (in order)
	    std::vector<NodePort*> mInputs;
	    std::vector<NodePort*> mOutputs;

	    // Port names by index (in order)
	    std::vector<std::string> mInputNames;
	    std::vector<std::string> mOutputNames;
	};
    }
}
