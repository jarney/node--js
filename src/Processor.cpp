#include "node--js/Processor.hpp"
#include "node--js/NodeGraph.hpp"
#include "node--js/NodeModule.hpp"
#include <optional>

using namespace NodeJS::core;

void
Processor::setNativeImpl(NodeTypeId aNodeTypeId, NodeProcessor processor)
{
    mNodeProcessors[aNodeTypeId] = processor;
}
        
ConnectionData
Processor::processGraph(const NodeGraph & graph)
{
    // Being in topological order ensures that each node
    // will have its prececessors calculated first
    // so it will have the input data it needs by the time
    // it is called.
    std::map<NodeId, ConnectionData> nodeData;
        
    std::optional<std::vector<NodeId>> maybeNodeIds = graph.getNodeIdsInTopologicalOrder();
    const std::vector<NodeId> & nodeIds = maybeNodeIds.value();

    ConnectionData lastData;

    if (nodeIds.size() == 0) {
	return lastData;
    }
    
    nodeData[nodeIds.at(0)] = ConnectionData();

    for (const NodeId & nodeId : nodeIds) {
        const Node & node = *graph.getNode(nodeId);

        // Get the type of the node.
        const NodeType & nodeType = node.getType();

        // When processing a node,
        // we're really getting all of the data from
        // prececessor nodes, so we need to shuttle it
        // to the right place.
        ConnectionData fromData;
        for (const Edge *edge : graph.getEdgesFrom(nodeId)) {
	    std::string data = nodeData[edge->toNode].getValue(edge->toPort);
            fromData.setValue(edge->fromPort, data);
	}
        ConnectionData toData;
        processNodeType(graph, nodeType, node, fromData, toData);
        nodeData[nodeId] = toData;
        lastData = toData;
    }
    return lastData;
}

void
Processor::processNodeType(
    const NodeGraph & graph,
    const NodeType & nodeType,
    const Node & node,
    const ConnectionData & fromData,
    ConnectionData & toData)
{
    if (nodeType.getType() == NodeType::Type::NATIVE) {
	NodeTypeId nodeTypeId = nodeType.getId();
	const auto nodeProcessorIt = mNodeProcessors.find(nodeTypeId);
	if (nodeProcessorIt == mNodeProcessors.end()) {
	    default_processor(node, fromData, toData);
	    return;
	}
	NodeProcessor processor = nodeProcessorIt->second;
	processor(node, fromData, toData);
    }
    else if (nodeType.getType() == NodeType::Type::GRAPH) {
	NodeModule & module = graph.getModule();
	NodeGraph *subgraph = module.getGraph(nodeType.getId());
	if (subgraph == nullptr) {
	    default_processor(node, fromData, toData);
	    return;
	}
	processGraph(*subgraph);
    }
}

void
Processor::default_processor(
    const Node & node,
    const ConnectionData & fromData,
    ConnectionData & toData
    )
{
    fprintf(stderr, "Default processing unregistered node %s\n", node.getType().getId().c_str());
}
