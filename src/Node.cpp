#include "node--js/Node.hpp"

using namespace NodeJS::core;

Node::Node(
    NodeId aId,
    const NodeType & aType,
    NodeGraph & aGraph,
    const ConnectionData & aData
    )
    : mId(aId)
    , mType(aType)
    , mGraph(aGraph)
    , mData(aData)
{}

const NodeId &
Node::getId() const
{ return mId; }

const NodeType &
Node::getType() const
{ return mType; }

NodeGraph &
Node::getGraph() const
{ return mGraph; }

ConnectionData &
Node::getData()
{ return mData; }
