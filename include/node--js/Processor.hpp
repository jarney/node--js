#pragma once

#include "node--js/ConnectionData.hpp"
#include "node--js/NodeType.hpp"
#include "node--js/Node.hpp"
#include <map>
#include <functional>

namespace NodeJS {
    namespace core {

	/**
	 * A node processor takes the information
	 * from a node and the input data and calculates
	 * a result into the 'toData'.
	 */
	class NodeProcessor {
	public:
	    NodeProcessor() = default;
	    virtual ~NodeProcessor() = default;
	    virtual void process(
		const Node & node,
		const ConnectionData & fromData,
		ConnectionData & toData
		) = 0;
	};

	class Processor {
	public:
	    Processor() = default;
	    virtual ~Processor() = default;

	    void setNativeImpl(NodeTypeId aNodeTypeId, std::unique_ptr<NodeProcessor> proc);

            void processGraph(
				const NodeGraph & graph,
				const ConnectionData & input,
				ConnectionData & output
			       );

	    /**
	     * Performs the process step on an individual node.
	     */
	    void processNodeType(
		const NodeGraph & graph,
		const NodeType & nodeType,
		const Node & node,
		const ConnectionData & input,
		ConnectionData & output);

	    static void default_processor(
		const Node & node,
		const ConnectionData & input,
		ConnectionData & output);
	    
	private:
	    std::map<NodeTypeId, std::unique_ptr<NodeProcessor>> mNodeProcessors;
	};
    }
}
