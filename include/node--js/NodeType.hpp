#pragma once

#include <string>
#include <map>
#include <vector>

#include "node--js/NodePort.hpp"

namespace NodeJS {
    namespace core {
	class NodeType {
	public:

	    typedef enum {
		PUBLIC,
		PRIVATE
	    } Visibility;

	    typedef enum {
		NATIVE,
		GRAPH
	    } Type;
	    
	    NodeType() = default;
	    ~NodeType() = default;

	    const std::string & getId() const;
	    void setId(std::string id);
	    
	    
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
