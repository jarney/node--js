

namespace NodeJS {
    namespace compiler {

	/**
	 * This performs the compilation stage of a NodeJS program.
	 * NodeJS programs are compiled by examining the graph
	 * by identifying an output node to process and then
	 * performing a topological sort of the graph to place
	 * the nodes in a list that is sorted in the order it
	 * needs to process in order to calculate the result.
	 *
	 * Once the nodes are in order, each node is processed
	 * by calling the node type's registered 'process' function
	 * for 'native' nodes, or by recursively processing the
	 * node type's graph for each of its output nodes.
	 */
	class Compiler {
	};
    }
}
