#include "node--js/Processor.hpp"
#include "node--js/NodeGraph.hpp"
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
            fromData.setValue(edge->fromPort, nodeData[edge->toNode].getValue(edge->toPort));
	}
        ConnectionData toData;
        processNodeType(nodeType, node, fromData, toData);
        nodeData[nodeId] = toData;
        lastData = toData;
    }
    return lastData;
}
/*
  def processNodeType(self, nodeType, node, fromData, toData):
        if nodeType.getType() == NodeType.Type.NATIVE:
            nodeTypeId = nodeType.getId()
            if nodeTypeId not in self.native_impl:
                self.defaultProcess(node, fromData, toData)
            else:
                processor = self.native_impl[nodeTypeId]
                processor(node, fromData, toData)
        elif nodeType.getType() == NodeType.Type.GRAPH:
            module = graph.getModule()
            subgraph = module.getGraph(nodeType.getId())
            self.processGraph(subgraph)
        
    def defaultProcess(self, node, fromData, toData):
        print("Processing node " + node.getType().getId())
*/
