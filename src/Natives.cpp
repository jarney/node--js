#include "node--js/Natives.hpp"

using namespace NodeJS::core;

#define DEFINE_PROCESSOR(name, id) \
std::string name::ID = id; \
void \
name::process(		   \
    Processor & processor, \
    const Node & node, \
    const ConnectionData & fromData, \
    ConnectionData & toData \
    )

// This provides a constant string literal.
DEFINE_PROCESSOR(NodeProcessor_string, "org.ensor.nodejs.core.string")
{
    toData.setValue("value", node.getData().getValue("value"));
}

// Get information from the node type (or its alias, more usefully)
// Is this really going to work?
// Still need to think about this some more.....
//DEFINE_PROCESSOR(NodeProcessor_getvalue, "org.ensor.nodejs.core.getvalue")
//{
//    std::string value = node.getType().getDefaultNodeData().getValue(fromData.getValue("key"));
//    toData.setValue("value", value);
//}

// This concatenates two strings
DEFINE_PROCESSOR(NodeProcessor_concatenate, "org.ensor.nodejs.core.concatenate")
{
    toData.setValue("out", fromData.getValue("a") + fromData.getValue("b"));
}

// The input data for the input node is entirely
// handled by the 'Processor' itself.
DEFINE_PROCESSOR(NodeProcessor_input, "org.ensor.nodejs.core.input")
{}

// Output has a 'secret' output port
// which just moves its input back to its
// output.  This is how the 'Processor'
// retrieves the final output of the graph.
DEFINE_PROCESSOR(NodeProcessor_output, "org.ensor.nodejs.core.output")
{
    toData = fromData;
}


