#pragma once

#include "node--js/Metadata.hpp"
#include "node--js/Edge.hpp"
#include "node--js/Node.hpp"
#include "node--js/Group.hpp"

#include <optional>
#include <set>

namespace NodeJS {
    namespace core {

	typedef std::string GroupId;
	
	class Group {
	public:
	    Group() = default;
	    ~Group() = default;

	    void addNode(NodeId aNodeId);
	    
	    void removeNode(NodeId aNodeId);

	    const std::set<NodeId> & getNodes() const;

	    bool contains(NodeId aNodeId) const;
	    
	private:
	    std::set<NodeId> mNodes;
	};
	
    }
}
