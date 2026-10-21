#include <node--js/gui/GraphModelAdapter.hpp>
#include <node--js/NodeModule.hpp>

#include <QtNodes/NodeDelegateModel>
#include <QtNodes/ConnectionIdUtils>
#include <QtNodes/NodeData>
#include <QtNodes/StyleCollection>

#include <QJsonArray>

#include <QPoint>
#include <QSize>

#include <unordered_set>
#include <stack>


using namespace NodeJS::core;
using namespace NodeJS::gui;

GraphModelAdapter::GraphModelAdapter(
    NodeJS::core::NodeGraph & aGraph,
    NodeEditorWidget & aEditorWidget,
    const std::map<std::string, NodeEditorWidget::WidgetFactory> & aWidgetFactory
    )
    : mGraph(aGraph)
    , mWidgets(std::make_unique<std::map<QtNodes::NodeId, QWidget *>>()) // This is a pointer to fool the const qualified.
    , mEditorWidget(aEditorWidget)
    , mWidgetFactory(aWidgetFactory)
{
    mID_nextNew = 0;
    for (const auto & it : mGraph.getNodes()) {
	mID_toGraph.insert(std::make_pair(mID_nextNew, it.first));
	mID_fromGraph.insert(std::make_pair(it.first, mID_nextNew));
	mID_nextNew++;
    }
}

QtNodes::NodeId
GraphModelAdapter::newNodeId()
{
    while (true) {
	if (mID_toGraph.count(mID_nextNew) == 0) {
	    return mID_nextNew++;
	}
	mID_nextNew++;
    }
}

std::unordered_set<QtNodes::NodeId>
GraphModelAdapter::allNodeIds() const
{
    std::unordered_set<QtNodes::NodeId> allNodes;
    for (const auto & it : mID_toGraph) {
	allNodes.insert(it.first);
    }
    return allNodes;
}


/**
 * A collection of all input and output connections for the given `nodeId`.
 */
std::unordered_set<QtNodes::ConnectionId>
GraphModelAdapter::allConnectionIds(QtNodes::NodeId const nodeId) const
{
    std::unordered_set<QtNodes::ConnectionId> allConnections;

    // Node does not exist, this is a problem.
    const auto & nodeIt = mID_toGraph.find(nodeId);
    if (nodeIt == mID_toGraph.end()) {
	fprintf(stderr, "THIS SHOULD NEVER HAPPEN, WE'RE MISSING A NODE IN THE ADAPTER'S MAP %d\n", nodeId);
	return allConnections;
    }
    std::string nodeIdGraph = nodeIt->second;


    
    const auto & edges = mGraph.getEdges();
    for (const auto & edgeIt : edges) {

	QtNodes::ConnectionId connection;
	const Edge *edge = edgeIt.second.get();
	
	const Node *toNode = mGraph.getNode(edge->toNode);
	const Node *fromNode = mGraph.getNode(edge->fromNode);
	if (toNode == nullptr || fromNode == nullptr) {
	    fprintf(stderr, "THIS SHOULD NEVER HAPPEN, NODE NOT EXISTING IN GRAPH (to node OR FROM NODE) %s %p or %s %p\n",
		    edge->toNode.c_str(), toNode,
		    edge->fromNode.c_str(), fromNode);
	    continue;
	}
	if (edge->fromNode == nodeIdGraph) {
	    const auto & toNodeIt = mID_fromGraph.find(edge->toNode);
	    if (toNodeIt == mID_fromGraph.end()) {
		fprintf(stderr, "THIS SHOULD NEVER HAPPEN, NODE NOT EXISTING IN ADAPTER'S MAP (to node) %s\n", edge->toNode.c_str());
		continue;
	    }
	    connection.outNodeId = nodeId;
	    connection.inNodeId = toNodeIt->second;
	}
	else if (edge->toNode == nodeIdGraph) {
	    const auto & fromNodeIt = mID_fromGraph.find(edge->fromNode);
	    if (fromNodeIt == mID_fromGraph.end()) {
		fprintf(stderr, "THIS SHOULD NEVER HAPPEN, NODE NOT EXISTING IN ADAPTER'S MAP (from node) %s\n", edge->fromNode.c_str());
		continue;
	    }
	    connection.outNodeId = fromNodeIt->second;
	    connection.inNodeId = nodeId;
	}
	else {
	    // This isn't one of our nodes, so don't worry about it.
	    continue;
	}
	connection.outPortIndex = fromNode->getOutputs().getPortIndex(edge->fromPort);
	connection.inPortIndex = toNode->getInputs().getPortIndex(edge->toPort);
	allConnections.insert(connection);
    }

    return allConnections;
}


std::unordered_set<QtNodes::ConnectionId>
GraphModelAdapter::connections(
    QtNodes::NodeId nodeId,
    QtNodes::PortType portType,
    QtNodes::PortIndex index) const
{
    std::unordered_set<QtNodes::ConnectionId> selectedConnections;

    const auto & connectionsIt = mID_toGraph.find(nodeId);
    const Node *connectionsNode = mGraph.getNode(connectionsIt->second);
    
    const auto allConnections = allConnectionIds(nodeId);
    for (const auto & connection : allConnections) {

	const auto & fromIt = mID_toGraph.find(connection.outNodeId);
	const auto & toIt = mID_toGraph.find(connection.inNodeId);
	if (fromIt == mID_toGraph.end() || toIt == mID_toGraph.end()) {
	    fprintf(stderr, "Invalid connection, no such nodes %d %d\n", connection.outNodeId, connection.inNodeId);
	    continue;
	}
	const Node *fromNode = mGraph.getNode(fromIt->second);
	const Node *toNode = mGraph.getNode(toIt->second);
	
	if (portType == QtNodes::PortType::In &&
	    nodeId == connection.inNodeId &&
	    index == connection.inPortIndex
	    ) {
	    selectedConnections.insert(connection);
	}
	else if (portType == QtNodes::PortType::Out &&
		 nodeId == connection.outNodeId &&
	         index == connection.outPortIndex
	    ) {
	    selectedConnections.insert(connection);
	}
    }
    return selectedConnections;
}

	    
bool
GraphModelAdapter::connectionExists(QtNodes::ConnectionId const connectionId) const
{
    const auto allConnections = allConnectionIds(connectionId.inNodeId);
    for (const auto & connection : allConnections) {
	if (connection == connectionId) return true;
    }
    return false;
}


QtNodes::NodeId
GraphModelAdapter::addNode(QString const nodeTypeName)
{
    // If we can't resolve the node type,
    // we cannot create the node.
    const NodeType *nodeType = mGraph.getModule().getNodeType(nodeTypeName.toStdString());
    if (nodeType == nullptr) {
	fprintf(stderr, "Creating node: %s, no such type exists\n", nodeTypeName.toStdString().c_str());
	return QtNodes::InvalidNodeId;
    }

    ConnectionData emptyData;
    Node &newNode = mGraph.newNode(*nodeType, nodeType->getLocalId(), emptyData);
    
    // Now that we have the node, we need to keep track of the IDs so we can map correctly.
    QtNodes::NodeId adapterNodeId = newNodeId();
    mID_toGraph.insert(std::make_pair(adapterNodeId, newNode.getId()));
    mID_fromGraph.insert(std::make_pair(newNode.getId(), adapterNodeId));

    Q_EMIT nodeCreated(adapterNodeId);
    
    return adapterNodeId;
}

// This is internal, we don't need to expose it.
// We may at some point, allow 'typecasting'
// node data, but this might not actually be a good idea.
static bool
dataTypeConnectionAllowed(const QtNodes::NodeDataType & outType, const QtNodes::NodeDataType & inType)
{
    return outType.id == inType.id;
}

bool
GraphModelAdapter::connectionPossible(QtNodes::ConnectionId const connectionId) const
{
    // Check if nodes exist
    if (!nodeExists(connectionId.outNodeId) || !nodeExists(connectionId.inNodeId)) {
	fprintf(stderr, "Connection not possible, nodes do not exist\n");
        return false;
    }
    
    // Check port bounds, i.e. that we do not connect non-existing port numbers
    auto checkPortBounds = [&](QtNodes::PortType const portType) {
	QtNodes::NodeId const nodeId = QtNodes::getNodeId(portType, connectionId);
        auto portCountRole = (portType == QtNodes::PortType::Out) ? QtNodes::NodeRole::OutPortCount
                                                         : QtNodes::NodeRole::InPortCount;

        std::size_t const portCount = nodeData(nodeId, portCountRole).toUInt();

        return QtNodes::getPortIndex(portType, connectionId) < portCount;
    };

    auto getDataType = [&](QtNodes::PortType const portType) {
        return portData(QtNodes::getNodeId(portType, connectionId),
                        portType,
                        QtNodes::getPortIndex(portType, connectionId),
                        QtNodes::PortRole::DataType)
            .value<QtNodes::NodeDataType>();
    };

    auto portVacant = [&](QtNodes::PortType const portType) {
        QtNodes::NodeId const nodeId = QtNodes::getNodeId(portType, connectionId);
        QtNodes::PortIndex const portIndex = QtNodes::getPortIndex(portType, connectionId);
        auto const connected = connections(nodeId, portType, portIndex);

        auto policy = portData(nodeId, portType, portIndex, QtNodes::PortRole::ConnectionPolicyRole)
	    .value<QtNodes::ConnectionPolicy>();
        return connected.empty() || (policy == QtNodes::ConnectionPolicy::Many);
    };

    bool typeCheck = dataTypeConnectionAllowed(getDataType(QtNodes::PortType::Out), getDataType(QtNodes::PortType::In));
    
    bool const basicChecks = typeCheck
                             && portVacant(QtNodes::PortType::Out) && portVacant(QtNodes::PortType::In)
                             && checkPortBounds(QtNodes::PortType::Out) && checkPortBounds(QtNodes::PortType::In);
    
    // In data-flow mode (this class) it's important to forbid graph loops.
    // We perform depth-first graph traversal starting from the "Input" port of
    // the given connection. We should never encounter the starting "Out" node.

    auto hasLoops = [this, &connectionId]() -> bool {
        std::stack<QtNodes::NodeId> filo;
        filo.push(connectionId.inNodeId);

        while (!filo.empty()) {
            auto id = filo.top();
            filo.pop();

            if (id == connectionId.outNodeId) { // LOOP!
                return true;
            }

            // Add out-connections to continue interations
            std::size_t const nOutPorts = nodeData(id, QtNodes::NodeRole::OutPortCount).toUInt();

            for (QtNodes::PortIndex index = 0; index < nOutPorts; ++index) {
                auto const &outConnectionIds = connections(id, QtNodes::PortType::Out, index);

                for (auto cid : outConnectionIds) {
                    filo.push(cid.inNodeId);
                }
            }
        }

        return false;
    };

    return basicChecks && (loopsEnabled() || !hasLoops());
}

bool
GraphModelAdapter::nodeExists(QtNodes::NodeId const nodeId) const
{
    return (mID_toGraph.find(nodeId) != mID_toGraph.end());
}

static float to_float(std::string str, float defaultValue)
{
    const char *xp = str.c_str();
    char *endx{};
    float value = strtof(xp, &endx);
    if (endx == xp) {
	value = defaultValue;
    }
    return value;
}

static int to_int(std::string str, int defaultValue)
{
    const char *xp = str.c_str();
    char *endx{};
    int value = strtod(xp, &endx);
    if (endx == xp) {
	value = defaultValue;
    }
    return value;
}


QVariant GraphModelAdapter::nodeData(QtNodes::NodeId nodeId, QtNodes::NodeRole role) const
{
    QVariant result;

    const auto & graphNodeIdIt = mID_toGraph.find(nodeId);
    if (graphNodeIdIt == mID_toGraph.end()) {
	return result;
    }

    Node *node = mGraph.getNode(graphNodeIdIt->second);
    if (node == nullptr) {
        return result;
    }

    ConnectionData & nodeMetadata = node->getMetadata().getMetadata(METADATA_NAMESPACE_GRAPH_EDITOR);
    const ConnectionData & nodeTypeMetadata = node->getType().getMetadata().getMetadata(METADATA_NAMESPACE_GRAPH_EDITOR);
    
    switch (role) {
    case QtNodes::NodeRole::Type: {
        result = QString::fromStdString(node->getId());
    } break;
    case QtNodes::NodeRole::Position: {
	std::string positionXStr = nodeMetadata.getValue("pos-x", "0.0");
	std::string positionYStr = nodeMetadata.getValue("pos-y", "0.0");
        QPointF pos(to_float(positionXStr, 0.0), to_float(positionYStr, 0.0));
        result = pos;
    } break;
    case QtNodes::NodeRole::Size: {
	// These get called way too often,
	// we should only be doing conversion like this
	// when we are ready to serialize.
	std::string positionXStr = nodeMetadata.getValue("width", "0");
	std::string positionYStr = nodeMetadata.getValue("height", "0");
        QSize size(to_int(positionXStr, 0), to_int(positionYStr, 0));
        result = size;
    } break;
    case QtNodes::NodeRole::CaptionVisible: {
	result = nodeTypeMetadata.hasValue("caption");
    } break;
    case QtNodes::NodeRole::Caption: {
	result = QString::fromStdString(nodeTypeMetadata.getValue("caption"));
    } break;
    case QtNodes::NodeRole::Style: {
	// We will probably never have a reason to change this.
        result = QtNodes::StyleCollection::nodeStyle().toJson().toVariantMap();
    } break;
    case QtNodes::NodeRole::InternalData: {
	// This has no purpose and should be removed.
    } break;
    case QtNodes::NodeRole::InPortCount: {
        result = QVariant::fromValue((int)node->getInputs().getCount());
    } break;
    case QtNodes::NodeRole::OutPortCount: {
        result = QVariant::fromValue((int)node->getOutputs().getCount());
    } break;
    case QtNodes::NodeRole::Widget: {
        // We want lazy initialization on the
	// widgets, but we don't want to give up the
	// const qualifier here, so our widget cache map needs
	// to be a pointer instead of a member.
	const auto & widgetIt = mWidgets->find(nodeId);
	if (widgetIt == mWidgets->end()) {
	    const auto & widgetFactoryIt = mWidgetFactory.find(node->getType().getFullyQualifiedId());
	    if (widgetFactoryIt != mWidgetFactory.end()) {
		QWidget *widget = widgetFactoryIt->second(mEditorWidget, *node);
		(*mWidgets)[nodeId] = widget;
		result = QVariant::fromValue(widget);
	    }
	}
	else {
	    result = QVariant::fromValue(widgetIt->second);
	}
    } break;
    case QtNodes::NodeRole::ValidationState: {
	// State is meaningless for our application right now unless
	// we do some additional error checking and validation.
        result = QVariant::fromValue(QtNodes::NodeValidationState::State::Valid);
    } break;
    case QtNodes::NodeRole::ProcessingStatus: {
	// State is meaningless for our application right now unless
	// we do some additional error checking and validation.
        result = QVariant::fromValue(QtNodes::NodeProcessingStatus::NoStatus);
    } break;
    case QtNodes::NodeRole::ProgressValue: {
        result = QString();
    } break;
    case QtNodes::NodeRole::LabelVisible: {
	result = true;  //nodeMetadata.hasValue("label");
    } break;
    case QtNodes::NodeRole::Label: {
	if (nodeMetadata.hasValue("label")) {
	    result = QString::fromStdString(nodeMetadata.getValue("label"));
	}
	else {
	    result = QString::fromStdString(node->getId());
	}
    } break;
    case QtNodes::NodeRole::LabelEditable: {
        result = true;
    } break;
    case QtNodes::NodeRole::Tooltip: {
	if (nodeTypeMetadata.hasValue("tooltip")) {
	    result = QString::fromStdString(nodeTypeMetadata.getValue("tooltip"));
	}
    } break;
    }

    return result;
}

bool
GraphModelAdapter::setNodeData(
    QtNodes::NodeId nodeId,
    QtNodes::NodeRole role,
    QVariant value)
{
    Q_UNUSED(nodeId);
    Q_UNUSED(role);
    Q_UNUSED(value);

    bool result = false;

    const auto & graphNodeIdIt = mID_toGraph.find(nodeId);
    if (graphNodeIdIt == mID_toGraph.end()) {
	fprintf(stderr, "Could not find graph node %d\n", nodeId);
	return result;
    }

    Node *node = mGraph.getNode(graphNodeIdIt->second);
    if (node == nullptr) {
	fprintf(stderr, "Could not get node metadata %s\n", graphNodeIdIt->second.c_str());
        return result;
    }

    ConnectionData & nodeMetadata = node->getMetadata().getMetadata(METADATA_NAMESPACE_GRAPH_EDITOR);
    
    switch (role) {
    case QtNodes::NodeRole::Type: {
    } break;
    case QtNodes::NodeRole::Position: {
        QPointF pos = value.value<QPointF>();
	nodeMetadata.setValue("pos-x", std::to_string(pos.x()));
	nodeMetadata.setValue("pos-y", std::to_string(pos.y()));
        Q_EMIT nodePositionUpdated(nodeId);
	result = true;
    } break;
    case QtNodes::NodeRole::Size: {
        QSize size = value.value<QSize>();
	nodeMetadata.setValue("width", std::to_string(size.width()));
	nodeMetadata.setValue("height", std::to_string(size.height()));
        result = true;
    } break;
    case QtNodes::NodeRole::CaptionVisible: {
    } break;
    case QtNodes::NodeRole::Caption: {
    } break;
    case QtNodes::NodeRole::Style: {
    } break;
    case QtNodes::NodeRole::InternalData: {
	// This has no purpose and should be removed.
    } break;
    case QtNodes::NodeRole::InPortCount: {
    } break;
    case QtNodes::NodeRole::OutPortCount: {
    } break;
    case QtNodes::NodeRole::Widget: {
    } break;
    case QtNodes::NodeRole::ValidationState: {
	// State is meaningless for our application right now unless
	// we do some additional error checking and validation.
        Q_EMIT nodeUpdated(nodeId);
    } break;
    case QtNodes::NodeRole::ProcessingStatus: {
	// State is meaningless for our application right now unless
	// we do some additional error checking and validation.
        Q_EMIT nodeUpdated(nodeId);
    } break;
    case QtNodes::NodeRole::ProgressValue: {
    } break;
    case QtNodes::NodeRole::LabelVisible: {
        Q_EMIT nodeUpdated(nodeId);
    } break;
    case QtNodes::NodeRole::Label: {
	nodeMetadata.setValue("label", value.toString().toStdString());
        Q_EMIT nodeUpdated(nodeId);
	result = true;
    } break;
    case QtNodes::NodeRole::LabelEditable: {
    } break;
    }

    return result;
}

QVariant
GraphModelAdapter::portData(
    QtNodes::NodeId nodeId,
    QtNodes::PortType portType,
    QtNodes::PortIndex portIndex,
    QtNodes::PortRole role) const
{
    QVariant result;

    const auto & graphNodeIdIt = mID_toGraph.find(nodeId);
    if (graphNodeIdIt == mID_toGraph.end()) {
	return result;
    }

    Node *node = mGraph.getNode(graphNodeIdIt->second);
    if (node == nullptr) {
        return result;
    }

    ConnectionData & nodeMetadata = node->getMetadata().getMetadata(METADATA_NAMESPACE_GRAPH_EDITOR);
    const ConnectionData & nodeTypeMetadata = node->getType().getMetadata().getMetadata(METADATA_NAMESPACE_GRAPH_EDITOR);
    const NamedPorts & portData = (portType == QtNodes::PortType::In) ? node->getInputs() : node->getOutputs();
    const NodePort * nodePort = portData.getByIndex(portIndex);
    if (!nodePort) {
	fprintf(stderr,
		"!!!! No such port %s: %s %d (%s)\n",
		graphNodeIdIt->second.c_str(),
		((portType == QtNodes::PortType::In) ? "in" : "out"),
		portIndex,
		portData.getName(portIndex).c_str());
	return result;
    }
    const ConnectionData & portMetadata = nodePort->getMetadata().getMetadata(METADATA_NAMESPACE_GRAPH_EDITOR);

    switch (role) {
    case QtNodes::PortRole::Data: {
	// We're not supporting runtime data flow in the editor right now.
    } break;
    case QtNodes::PortRole::DataType: {
	const DataType *dataType = mGraph.getModule().getDataType(nodePort->getDataType());
	if (dataType) {
	    //fprintf(stderr, "node %s port %s data type %s\n",
	    //      graphNodeIdIt->second.c_str(),
	    //      portData.getName(portIndex).c_str(),
	    //      dataType->getFullyQualifiedId().c_str()
	    //      );
	    QtNodes::NodeDataType ndt{
				QString::fromStdString(dataType->getFullyQualifiedId()),
				QString::fromStdString(dataType->getName())
	    };
	    result = QVariant::fromValue(ndt);
	}
	else {
	    fprintf(stderr, "No port data for this port %s\n", portData.getName(portIndex).c_str());
	    QtNodes::NodeDataType ndt{
		"generic", "generic"
	    };
	    result = QVariant::fromValue(ndt);
	}
    } break;

    case QtNodes::PortRole::ConnectionPolicyRole: {
	QtNodes::ConnectionPolicy policy = nodePort->getConnectionPolicy() == NodePort::ConnectionPolicy::One ?
	    QtNodes::ConnectionPolicy::One : QtNodes::ConnectionPolicy::Many;
	result = QVariant::fromValue(policy);
    } break;
    case QtNodes::PortRole::CaptionVisible: {
	return portMetadata.hasValue("caption");
    } break;
    case QtNodes::PortRole::Caption: {
	return QString::fromStdString(portData.getName(portIndex));
	//return QString::fromStdString(portMetadata.getValue("caption"));
    } break;
    }
    
    return result;
}

bool
GraphModelAdapter::setPortData(
    QtNodes::NodeId nodeId,
    QtNodes::PortType portType,
    QtNodes::PortIndex portIndex,
    QVariant const &value,
    QtNodes::PortRole role
    )
{
    // This isn't really used since we don't set port-level
    // data during editing.
    return true;
}

std::map<QtNodes::GroupId, std::vector<QtNodes::NodeId>>
GraphModelAdapter::getGroups() const
{
    std::map<QtNodes::GroupId, std::vector<QtNodes::NodeId>> groups;
    QtNodes::GroupId groupId = 0;

#if 0
    // TODO: Get groups from metadata, it's no longer a part
    // of the core data model, it's a user/gui bolt-on.
    for (const auto & graphGroupIt : mGraph.getGroups()) {
	for (const auto & nodeIt : graphGroupIt.second->getNodes()) {
	    const auto & nodeIdIt = mID_fromGraph.find(nodeIt);
	    if (nodeIdIt == mID_fromGraph.end()) {
		continue;
	    }
	    groups[groupId].push_back(nodeIdIt->second);
	}
    }
#endif
    
    return groups;
}


bool
GraphModelAdapter::deleteNode(QtNodes::NodeId const nodeId)
{
    // Delete all connections:
    auto connectionIds = allConnectionIds(nodeId);
    for (auto &cId : connectionIds) {
        deleteConnection(cId);
    }
    
    const auto & it = mID_toGraph.find(nodeId);
    if (it == mID_toGraph.end()) {
	return false;
    }
    NodeId graphNodeId = it->second;
    mGraph.removeNode(graphNodeId);
    mID_fromGraph.erase(graphNodeId);
    mID_toGraph.erase(nodeId);
    // Widget will die when we delete it and may need to be re-created in the event of an undo.
    mWidgets->erase(nodeId); 

    Q_EMIT nodeDeleted(nodeId);

    return true;
}

void
GraphModelAdapter::addConnection(QtNodes::ConnectionId const connectionId)
{
    const auto & fromNodeIt = mID_toGraph.find(connectionId.outNodeId);
    const auto & toNodeIt = mID_toGraph.find(connectionId.inNodeId);
    if (fromNodeIt == mID_toGraph.end() ||
	toNodeIt == mID_toGraph.end()) {
	fprintf(stderr, "NODES COULD NOT BE RESOLVED WHEN CREATING CONNECTION\n");
	return;
    }
    
    NodeId fromNodeId = fromNodeIt->second;
    NodeId toNodeId = toNodeIt->second;

    Node *fromNode = mGraph.getNode(fromNodeId);
    Node *toNode = mGraph.getNode(toNodeId);

    Q_EMIT connectionCreated(connectionId);

    std::optional<EdgeId> optEdge = mGraph.newEdge(
	fromNodeId,
	fromNode->getOutputs().getName(connectionId.outPortIndex),
	toNodeId,
	toNode->getInputs().getName(connectionId.inPortIndex)
	);
}

bool
GraphModelAdapter::deleteConnection(QtNodes::ConnectionId const connectionId)
{
    const auto & fromNodeIt = mID_toGraph.find(connectionId.outNodeId);
    const auto & toNodeIt = mID_toGraph.find(connectionId.inNodeId);
    if (fromNodeIt == mID_toGraph.end() ||
	toNodeIt == mID_toGraph.end()) {
	fprintf(stderr, "NODES COULD NOT BE RESOLVED WHEN DELETING CONNECTION\n");
	return false;
    }
    
    NodeId fromNodeId = fromNodeIt->second;
    NodeId toNodeId = toNodeIt->second;

    Node *fromNode = mGraph.getNode(fromNodeId);
    Node *toNode = mGraph.getNode(toNodeId);

    if (!fromNode || !toNode) {
	fprintf(stderr, "NODES COULD NOT BE RESOLVED WHEN DELETING CONNECTION, not in map\n");
	return false;
    }

    if (!mGraph.hasEdge(
	    fromNodeId,
	    fromNode->getOutputs().getName(connectionId.outPortIndex),
	    toNodeId,
	    toNode->getInputs().getName(connectionId.inPortIndex)
	    )) {
	return false;
    }
    
    mGraph.removeEdge(
	fromNodeId,
	fromNode->getOutputs().getName(connectionId.outPortIndex),
	toNodeId,
	toNode->getInputs().getName(connectionId.inPortIndex)
	);

    Q_EMIT connectionDeleted(connectionId);

    return true;
}
const NodeJS::core::NodeGraph &
GraphModelAdapter::getNodeGraph() const
{
    return mGraph;
}

// These serializers don't need to be bullet-proof because
// they're only used to manage state inside the editor.
// Their content should never be used outside of the
// editor runtime or saved to persistent storage,
// so we don't have to worry too much about versioning
// or keeping backward-compatible interfaces here.

static void
saveConnectionData(QJsonObject & jsonData, const ConnectionData & data)
{
    for (const auto & it : data.getData()) {
	jsonData[QString::fromStdString(it.first)] = QString::fromStdString(it.second);
    }
}

static void
loadConnectionData(ConnectionData & data, const QJsonObject & jsonData)
{
    for (const auto & key : jsonData.keys()) {
	data.setValue(key.toStdString(), jsonData[key].toString().toStdString());
    }
}

static void
saveMetadata(QJsonArray & jsonMetadata, const Metadata & metadata)
{
    for (const auto & key : metadata.getNamespaces()) {
	QJsonObject metadataElement;
	metadataElement["namespace"] = QString::fromStdString(key);

	QJsonObject metadataDataJson;
	saveConnectionData(metadataDataJson, metadata.getMetadata(key));
	metadataElement["data"] = metadataDataJson;
	jsonMetadata.append(metadataElement);
    }
}
static void
loadMetadata(Metadata & metadata, const QJsonArray & jsonMetadata)
{
    for (QJsonValue val : jsonMetadata) {
	QJsonObject metadataElement = val.toObject();
	ConnectionData & data = metadata.getMetadata(metadataElement["namespace"].toString().toStdString());
	loadConnectionData(data, metadataElement["data"].toObject());
    }
}

static void
saveNamedPorts(QJsonArray & jsonPorts, const NamedPorts & namedPorts)
{
    for (size_t i = 0; i < namedPorts.getCount(); i++) {
	QJsonObject jsonPort;
	const NodePort *port = namedPorts.getByIndex(i);
	
	jsonPort["name"] = QString::fromStdString(namedPorts.getName(i));
	jsonPort["data-type"] = QString::fromStdString(port->getDataType());
	jsonPort["description"] = QString::fromStdString(port->getDescription());
	jsonPort["connection-policy"] = static_cast<qint32>(port->getConnectionPolicy());
	QJsonArray jsonMetadata;
	saveMetadata(jsonMetadata, port->getMetadata());
	jsonPort["metadata"] = jsonMetadata;
	jsonPorts.append(jsonPort);
    }
}

static void
loadNamedPorts(NamedPorts & namedPorts, const QJsonArray & jsonPorts)
{
    for (QJsonValue val : jsonPorts) {
	QJsonObject jsonPort = val.toObject();

	std::unique_ptr<NodePort> port = std::make_unique<NodePort>(
	    jsonPort["data-type"].toString().toStdString(),
	    jsonPort["description"].toString().toStdString(),
	    static_cast<NodePort::ConnectionPolicy>(jsonPort["connection-policy"].toInt())
	    );
	loadMetadata(port->getMetadata(), jsonPort["metadata"].toArray());
        namedPorts.addPort(jsonPort["name"].toString().toStdString(), std::move(port));
    }
}

/**
 * Used mainly in the 'undo/redo' stack for operations on
 * nodes and connections.
 */
QJsonObject
GraphModelAdapter::saveNode(QtNodes::NodeId const nodeId) const
{
    QJsonObject nodeJson;

    auto const nodeIt = mID_toGraph.find(nodeId);
    if (nodeIt == mID_toGraph.end()) {
        return nodeJson;
    }
    std::string nodeIdGraph = nodeIt->second;
    Node *node = mGraph.getNode(nodeIdGraph);

    nodeJson["id"] = static_cast<qint64>(nodeId);
    nodeJson["type"] = QString::fromStdString(node->getType().getFullyQualifiedId());

    QJsonObject data;
    saveConnectionData(data, node->getData());
    nodeJson["data"] = data;

    QJsonArray metadata;
    saveMetadata(metadata, node->getMetadata());
    nodeJson["metadata"] = metadata;
    
    if (node->hasOverrideInputs()) {
	QJsonArray inputPorts;
	saveNamedPorts(inputPorts, node->getOverrideInputs());
	nodeJson["inputPorts"] = inputPorts;
    }
    if (node->hasOverrideOutputs()) {
	QJsonArray outputPorts;
	saveNamedPorts(outputPorts, node->getOverrideOutputs());
	nodeJson["outputPorts"] = outputPorts;
    }
    
    QPointF const pos = nodeData(nodeId, QtNodes::NodeRole::Position).value<QPointF>();
    QJsonObject posJson;
    posJson["x"] = pos.x();
    posJson["y"] = pos.y();
    nodeJson["position"] = posJson;

    return nodeJson;
}

void
GraphModelAdapter::loadNode(QJsonObject const &nodeJson)
{
    // Possibility of the id clash when reading it from json and not generating a
    // new value.
    // 1. When restoring a scene from a file.
    // Conflict is not possible because the scene must be cleared by the time of
    // loading.
    // 2. When undoing the deletion command.  Conflict is not possible
    // because all the new ids were created past the removed nodes.
    QtNodes::NodeId restoredNodeId = nodeJson["id"].toInt();
    NodeTypeId nodeTypeId = nodeJson["type"].toString().toStdString();

    mID_nextNew = std::max(mID_nextNew, restoredNodeId + 1);

    const NodeType *nodeType = mGraph.getModule().getNodeType(nodeTypeId);
    if (nodeType == nullptr) {
	fprintf(stderr, "Loading node: %s, no such type exists\n", nodeTypeId.c_str());
	return;
    }

    ConnectionData initialData;
    loadConnectionData(initialData, nodeJson["data"].toObject());
    // TODO: Pull this out of our json data.
    Node &newNode = mGraph.newNode(*nodeType, nodeType->getLocalId(), initialData);
    mID_toGraph[restoredNodeId] = newNode.getId();
    mID_fromGraph[newNode.getId()] = restoredNodeId;

    newNode.setOverrideInputs(nodeJson.contains("inputPorts"));
    if (newNode.hasOverrideInputs()) {
	loadNamedPorts(newNode.getOverrideInputs(), nodeJson["inputPorts"].toArray());
    }

    newNode.setOverrideOutputs(nodeJson.contains("outputPorts"));
    if (newNode.hasOverrideOutputs()) {
	loadNamedPorts(newNode.getOverrideOutputs(), nodeJson["outputPorts"].toArray());
    }

    loadMetadata(newNode.getMetadata(), nodeJson["metadata"].toArray());
    
    
    Q_EMIT nodeCreated(restoredNodeId);

    QJsonObject posJson = nodeJson["position"].toObject();
    QPointF const pos(posJson["x"].toDouble(), posJson["y"].toDouble());
    setNodeData(restoredNodeId, QtNodes::NodeRole::Position, pos);
}

