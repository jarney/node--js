#include "node--js/Processor.hpp"
#include "node--js/NodeGraph.hpp"
#include "node--js/NodeModule.hpp"
#include "node--js/Natives.hpp"
#include "node--js/QualifiedIdentifier.hpp"
#include <optional>

using namespace NodeJS::core;

void
Processor::setNativeImpl(NodeTypeId aNodeTypeId, std::unique_ptr<NodeProcessor> processor)
{
    mNodeProcessors[aNodeTypeId] = std::move(processor);
}

#define __NODEJS_DEBUGGING_PRINTS

#ifdef __NODEJS_DEBUGGING_PRINTS
static void
dumpDict(std::string msg, const ConnectionData & data)
{
    fprintf(stderr, "%s\n", msg.c_str());
    for (const auto & it : data.getData()) {
	fprintf(stderr, "        %s : %s\n", it.first.c_str(), it.second.c_str());
    }
}
#endif

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

    // Find the 'input' and 'output' nodes for the graph.
    // These are mandatory and should be:
    // * Mapped to the 'org.ensor.nodejs.core.[input|output]' node types.
    // * Should be unique in the graph.
    // * Should match type for type the corresponding
    //   node type when called from a node type.
    // All of this should be validated when the graph is serialized/edited.

    std::optional<NodeId> inputNodeId;
    std::optional<NodeId> outputNodeId;

    static const std::string INPUT_NODE_TYPE("org.ensor.nodejs.core.input");
    static const std::string OUTPUT_NODE_TYPE("org.ensor.nodejs.core.output");

    for (const NodeId & nodeId : nodeIds) {
	const Node & node = *graph.getNode(nodeId);
	if (node.getType().getFullyQualifiedId() == NodeProcessor_input::ID) {
	    if (inputNodeId.has_value()) {
		fprintf(stderr, "Could not process graph, it was not valid\n");
		fprintf(stderr, "It has a duplicate input node\n");
		return;
	    }
	    inputNodeId = nodeId;
	}
	if (node.getType().getFullyQualifiedId() == NodeProcessor_output::ID) {
	    if (outputNodeId.has_value()) {
		fprintf(stderr, "Could not process graph, it was not valid\n");
		fprintf(stderr, "It has a duplicate output node\n");
		return;
	    }
	    outputNodeId = nodeId;
	}
    }
    if (!inputNodeId.has_value()) {
	fprintf(stderr, "Could not process graph, missing input node\n");
	return;
    }
    if (!outputNodeId.has_value()) {
	fprintf(stderr, "Could not process graph, missing output node\n");
	return;
    }
    
    nodeData[inputNodeId.value()] = input;
    dumpDict("Graph input:", input);
    
    for (const NodeId & nodeId : nodeIds) {
        const Node & node = *graph.getNode(nodeId);

        // Get the type of the node.
        const NodeType & nodeType = node.getType();

        // When processing a node,
        // we're really getting all of the data from
        // prececessor nodes, so we need to shuttle it
        // to the right place.  The big exception is the
	// 'input' node which gets it directly from
	// the graph itself because it has no previous edge.
	if (nodeType.getFullyQualifiedId() == "org.ensor.nodejs.core.input") {
	    continue;
	}
	ConnectionData fromData;
	for (const Edge *edge : graph.getEdgesTo(nodeId)) {
	    std::string data = nodeData[edge->fromNode].getValue(edge->fromPort);
	    fromData.appendValue(edge->toPort, data);
	    //fprintf(stderr, "Taking data from %s:%s -> %s:%s = %s\n",
	    //      edge->fromNode.c_str(), edge->fromPort.c_str(),
            //      edge->toNode.c_str(), edge->toPort.c_str(),
            //      data.c_str());
	}
	ConnectionData toData;
	processNodeType(graph, nodeType, node, fromData, toData);
	nodeData[nodeId] = toData;
    }
    output = nodeData[outputNodeId.value()];
    dumpDict("Graph output:", output);
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
	// Node here needs to be fully qualified
	// in order to make sure we're running a truly unique
	// processor per node.
	NodeTypeId nodeTypeId = nodeType.getFullyQualifiedId();
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
    else if (nodeType.getType() == NodeType::Type::ALIAS) {
	const NodeType *aliasForNode = graph.getModule().getNodeType(nodeType.getAliasNodeTypeId());
	if (!aliasForNode) {
	    default_processor(*this, node, fromData, toData);
	    return;
	}
	processNodeType(graph, *aliasForNode, node, fromData, toData);
    }
    else if (nodeType.getType() == NodeType::Type::GRAPH) {
	NodeModule & nodeModule = graph.getModule();
	
	QualifiedIdentifier qualifiedName(nodeType.getGraphId());
	std::string qualifiedGraphId;
	if (qualifiedName.isQualified()) {
	    qualifiedGraphId = nodeType.getGraphId();
	}
	else {
	    qualifiedGraphId = nodeType.getPackage() + "." + qualifiedName.getName();
	    fprintf(stderr, "Graph is not qualified, so we should qualify it with our package name %s\n", qualifiedGraphId.c_str());
	}
	
	NodeGraph *subgraph = nodeModule.getGraph(qualifiedGraphId);
	if (subgraph == nullptr) {
	    fprintf(stderr, "No such graph found %s\n", nodeType.getGraphId().c_str());
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

	// Move data from here into ports if available to
	// provide "extra" inputs to graphs.
	ConnectionData graphInputData;
	for (const auto & it : nodeType.getGraphArguments()) {
	    const NodeTypeGraphArgument & arg = it.second;
	    switch (arg.argType) {
	    case NodeTypeGraphArgument::NodeTypeGraphArgument::NODE_TYPE_DATA:
		graphInputData.setValue(arg.graphPortName, nodeType.getDefaultNodeData().getValue(arg.nodePortName));
		fprintf(stderr, "Sending data from node type %s %s\n",
			arg.graphPortName.c_str(),
			nodeType.getDefaultNodeData().getValue(arg.nodePortName).c_str());
		break;
	    case NodeTypeGraphArgument::NodeTypeGraphArgument::NODE_DATA:
		graphInputData.setValue(arg.graphPortName, node.getData().getValue(arg.nodePortName));
		fprintf(stderr, "Sending data from node itself %s %s\n",
			arg.graphPortName.c_str(),
			node.getData().getValue(arg.nodePortName).c_str());
		break;
	    case NodeTypeGraphArgument::NodeTypeGraphArgument::NODE_PORT:
		graphInputData.setValue(arg.graphPortName, fromData.getValue(arg.nodePortName));
		fprintf(stderr, "Sending data from node's input port %s %s\n",
			arg.graphPortName.c_str(),
			node.getData().getValue(arg.nodePortName).c_str());
		break;
	    }
	}
	ConnectionData graphOutputData;
	processGraph(*subgraph, graphInputData, graphOutputData);
	for (const auto & it : nodeType.getGraphResults()) {
	    const NodeTypeGraphArgument & arg = it.second;
	    toData.setValue(arg.nodePortName, graphOutputData.getValue(arg.graphPortName));
	}
	
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
    fprintf(stderr, "Default processing unregistered node %s\n", node.getType().getFullyQualifiedId().c_str());
}
