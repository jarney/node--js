#pragma once

#include "node--js/Metadata.hpp"
#include "node--js/NodeType.hpp"
#include "node--js/ConnectionData.hpp"
#include "node--js/NamedPorts.hpp"

namespace NodeJS {
    namespace core {

	/**
	 * A NodeId is a unique identifier associated
	 * with the node.
	 */
	typedef std::string NodeId;
	
	class NodeGraph;

	/**
	 * A node represents a vertex in a processing graph.
	 * Each node has a type which indicates the way a node will
	 * process data from input and produce an output.  Each node
	 * may also carry data inside it which effectively allows
	 * parameterization of the processing.  This acts as a
	 * "compile-time" configuration of the node.  All "run-time"
	 * configuration is accomplished by input ports.
	 *
	 * A node has a reference back to the graph it resides in
	 * so that it can inspect other nodes if needed.
	 */
	class Node {
	public:
	    Node(
		NodeId aId,
		const NodeType & aType,
		NodeGraph & aGraph,
		const ConnectionData & aData
		);
	    virtual ~Node() = default;

	    const NodeId & getId() const;
	    
	    const NodeType & getType() const;

	    /**
	     * These provide the 'official' access to ports and indices.
	     * In most cases, we delegate down to the node type,
	     * but in some cases, the node itself decides what ports
	     * it exposes.  If we call addInputPort or addOutputPort,
	     * we are overriding the ports from the node type
	     * with our own ports.
	     */
	    const NamedPorts & getInputs() const;
	    const NamedPorts & getOutputs() const;
	    
	    NamedPorts & getOverrideInputs();
	    NamedPorts & getOverrideOutputs();

	    const NamedPorts & getOverrideInputs() const;
	    const NamedPorts & getOverrideOutputs() const;

	    void setOverrideInputs(bool aOverrideInputs);
	    void setOverrideOutputs(bool aOverrideOutputs);
	    bool hasOverrideInputs() const;
	    bool hasOverrideOutputs() const;

	    NodeGraph & getGraph() const;

	    ConnectionData & getData();
	    
	    const ConnectionData & getData() const;

	    /**
	     * Metadata is data that can be associated
	     * with a node module that can be used by other
	     * extensions to the system.
	     */
	    const Metadata & getMetadata(void) const;
	    Metadata & getMetadata(void);

	private:
	    // Data purely about the abstract node
	    // that is the same for each instance.  Factor this out
	    // to a node-type class.
	    NodeId mId;
	    const NodeType & mType;
	    NodeGraph & mGraph;
	    ConnectionData mData;

	    bool mOverrideInputs;
	    bool mOverrideOutputs;
	    // Ports by name

	    NamedPorts mInputs;
	    NamedPorts mOutputs;
	    
	    Metadata mMetadata;
	};
	
    } // End core
} // End JNodes
