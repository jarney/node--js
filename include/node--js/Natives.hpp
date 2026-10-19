#pragma once

#include "node--js/Processor.hpp"

namespace NodeJS {
    namespace core {

#define _NODEJS_CORE_DECLARE_PROCESSOR(name)	    \
class name : public NodeProcessor {                 \
public:                                             \
    static std::string ID;                          \
    void process(                                   \
	Processor & processor,                      \
	const Node & node,                          \
	const ConnectionData & fromData,            \
	ConnectionData & toData                     \
	);                                          \
}

/*
 * Native implementations of core/important nodes
 * that are specifically related to the core system.
 * Lots of other nodes are implemented using these
 * as the fundamental basis.
 */
_NODEJS_CORE_DECLARE_PROCESSOR(NodeProcessor_string);
_NODEJS_CORE_DECLARE_PROCESSOR(NodeProcessor_concatenate);
_NODEJS_CORE_DECLARE_PROCESSOR(NodeProcessor_input);
_NODEJS_CORE_DECLARE_PROCESSOR(NodeProcessor_output);

#undef _NODEJS_CORE_DECLARE_PROCESSOR

    }
}
