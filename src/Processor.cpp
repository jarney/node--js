#include "node--js/Processor.hpp"
#include "node--js/NodeGraph.hpp"
#include "node--js/NodeModule.hpp"
#include <optional>

using namespace NodeJS::core;

void
Processor::setNativeImpl(NodeTypeId aNodeTypeId, std::unique_ptr<NodeProcessor> processor)
{
    mNodeProcessors[aNodeTypeId] = std::move(processor);
}
        
static void
dumpDict(std::string msg, const ConnectionData & data)
{
    fprintf(stderr, "%s\n", msg.c_str());
    for (const auto & it : data.getData()) {
	fprintf(stderr, "        %s : %s\n", it.first.c_str(), it.second.c_str());
    }
}

void
Processor::processGraph(const NodeGraph & graph, const ConnectionData & input, ConnectionData & output)
{
    // Being in topological order ensures that each node
    // will have its prececessors calculated first
    // so it will have the input data it needs by the time
    // it is called.
    std::map<NodeId, ConnectionData> nodeData;
        
    std::optional<std::vector<NodeId>> maybeNodeIds = graph.getNodeIdsInTopologicalOrder();
    const std::vector<NodeId> & nodeIds = maybeNodeIds.value();

    if (nodeIds.size() == 0) {
        return;
    }

    ConnectionData lastData;
    nodeData[nodeIds.at(0)] = input;

    for (const NodeId & nodeId : nodeIds) {
	//fprintf(stderr, "Processing node %s\n", nodeId.c_str());
        const Node & node = *graph.getNode(nodeId);

        // Get the type of the node.
        const NodeType & nodeType = node.getType();

        // When processing a node,
        // we're really getting all of the data from
        // prececessor nodes, so we need to shuttle it
        // to the right place.
        ConnectionData fromData;
        for (const Edge *edge : graph.getEdgesTo(nodeId)) {
	    std::string data = nodeData[edge->fromNode].getValue(edge->fromPort);
            fromData.appendValue(edge->toPort, data);
	    //fprintf(stderr, "Taking data from %s:%s -> %s:%s = %s\n",
	    //      edge->fromNode.c_str(), edge->fromPort.c_str(),
	    //	    edge->toNode.c_str(), edge->toPort.c_str(),
	    //	    data.c_str());
	}
        ConnectionData toData;
        processNodeType(graph, nodeType, node, fromData, toData);
        nodeData[nodeId] = toData;
        lastData = toData;
    }
    output = lastData;
    //dumpDict("Graph output:", output);
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
	    default_processor(*this, node, fromData, toData);
	    return;
	}
	//fprintf(stderr, "Processing node type %s\n", nodeTypeId.c_str());
	//dumpDict("input:", fromData);
	NodeProcessor & processor = *nodeProcessorIt->second.get();
	processor.process(*this, node, fromData, toData);
	//dumpDict("output: ", toData);
    }
    else if (nodeType.getType() == NodeType::Type::GRAPH) {
	NodeModule & module = graph.getModule();
	NodeGraph *subgraph = module.getGraph(nodeType.getId());
	if (subgraph == nullptr) {
	    default_processor(*this, node, fromData, toData);
	    return;
	}
	// Rules:
	// - Only one terminal node called "output".
	// - Each output corresponds to the node type's output ports.
	// - Each input corresponds to the node type's input ports.
	// All of this implies:
	// - Assignment must produce a value (an assignment).
	// - Module Instantiation must be a value by itself (this is 'geometry')
	// - Function definition is also its own thing producing a value.
	// - Module definition also produces its own thing.
	processGraph(*subgraph, fromData, toData);
    }
}

void
Processor::default_processor(
    Processor & processor,
    const Node & node,
    const ConnectionData & input,
    ConnectionData & output
    )
{
    fprintf(stderr, "Default processing unregistered node %s\n", node.getType().getId().c_str());
}
