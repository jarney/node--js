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
	typedef std::function<void(
	    const Node & node,
	    const ConnectionData & fromData,
	    ConnectionData & toData
	    )> NodeProcessor;

	class Processor {
	public:
	    Processor() = default;
	    virtual ~Processor() = default;

	    void setNativeImpl(NodeTypeId aNodeTypeId, NodeProcessor proc);

	    ConnectionData processGraph(const NodeGraph & graph);

	    /**
	     * Performs the process step on an individual node.
	     */
	    void processNodeType(
		const NodeType & nodeType,
		const Node & node,
		const ConnectionData & fromData,
		ConnectionData & toData);

	    static void default_processor(
		const Node & node,
		const ConnectionData & fromData,
		ConnectionData & toData);
	    
	private:
	    std::map<NodeTypeId, NodeProcessor> mNodeProcessors;
	};
    }
}
