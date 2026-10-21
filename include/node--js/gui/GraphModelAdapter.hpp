#pragma once

#include <memory>
#include <unordered_map>

#include <QString>
#include <QJsonObject>

#include <QtNodes/Definitions>
#include <QtNodes/AbstractGraphModel>

#include <node--js/Node.hpp>
#include <node--js/NodeGraph.hpp>

#include <node--js/gui/NodeEditorWidget.hpp>

namespace NodeJS {
    namespace gui {
	static const std::string METADATA_NAMESPACE_GRAPH_EDITOR = "http://jarney.github.io/nodejs/editor";

	
	class  GraphModelAdapter : public QtNodes::AbstractGraphModel {
	    Q_OBJECT
	public:
	    GraphModelAdapter(
		NodeJS::core::NodeGraph & aGraph,
		NodeEditorWidget & aEditorWidget,
		const std::map<std::string, NodeEditorWidget::WidgetFactory> & aWidgetFactory
		);
			      
	    /// Generates a new unique QtNodes::NodeId.
	    virtual QtNodes::NodeId newNodeId();
	    
	    /// @brief Returns the full set of unique Node Ids.
	    /**
	     * Model creator is responsible for generating unique `unsigned int`
	     * Ids for all the nodes in the graph. From an Id it should be
	     * possible to trace back to the model's internal representation of
	     * the node.
	     */
	    virtual std::unordered_set<QtNodes::NodeId> allNodeIds() const;
	    
	    /**
	     * A collection of all input and output connections for the given `nodeId`.
	     */
	    virtual std::unordered_set<QtNodes::ConnectionId> allConnectionIds(QtNodes::NodeId const nodeId) const;
	    
	    /// @brief Returns all connected Node Ids for given port.
	    /**
	     * The returned set of nodes and port indices correspond to the type
	     * opposite to the given `portType`.
	     */
	    virtual std::unordered_set<QtNodes::ConnectionId> connections(QtNodes::NodeId nodeId,
								 QtNodes::PortType portType,
								 QtNodes::PortIndex index) const;
	    
	    /// Checks if two nodes with the given `connectionId` are connected.
	    virtual bool connectionExists(QtNodes::ConnectionId const connectionId) const;
	    
	    /// Creates a new node instance in the derived class.
	    /**
	     * The model is responsible for generating a unique `QtNodes::NodeId`.
	     * @param[in] nodeType is free to be used and interpreted by the
	     * model on its own, it helps to distinguish between possible node
	     * types and create a correct instance inside.
	     */
	    virtual QtNodes::NodeId addNode(QString const nodeType = QString());
	    
	    /// Model decides if a conection with a given connection Id possible.
	    /**
	     * The default implementation compares corresponding data types.
	     *
	     * It is possible to override the function and connect non-equal
	     * data types.
	     */
	    virtual bool connectionPossible(QtNodes::ConnectionId const connectionId) const;
	    
	    /**
	     * @brief Creates a new connection between two nodes.
	     *
	     * Default implementation emits signal
	     * `connectionCreated(connectionId)`
	     *
	     * In the derived classes user must emite the signal to notify the
	     * scene about the changes.
	     */
	    virtual void addConnection(QtNodes::ConnectionId const connectionId);
	    
	    /**
	     * @returns `true` if there is data in the model associated with the
	     * given `nodeId`.
	     */
	    virtual bool nodeExists(QtNodes::NodeId const nodeId) const;
	    
	    /// @brief Returns node-related data for requested QtNodes::NodeRole.
	    /**
	     * @returns Node Caption, Node Caption Visibility, Node Position etc.
	     */
	    virtual QVariant nodeData(QtNodes::NodeId nodeId, QtNodes::NodeRole role) const;
	    
	    /**
	     * @brief Sets node properties.
	     *
	     * Sets: Node Caption, Node Caption Visibility,
	     * Shyle, State, Node Position etc.
	     * @see QtNodes::NodeRole.
	     */
	    virtual bool setNodeData(QtNodes::NodeId nodeId, QtNodes::NodeRole role, QVariant value);
	    
	    /**
	     * @brief Returns port-related data for requested QtNodes::NodeRole.
	     *
	     * @returns Port Data Type, Port Data, Connection Policy, Port
	     * Caption.
	     */
	    virtual QVariant portData(QtNodes::NodeId nodeId,
				      QtNodes::PortType portType,
				      QtNodes::PortIndex portIndex,
				      QtNodes::PortRole role) const;
	    
	    virtual bool setPortData(QtNodes::NodeId nodeId,
				     QtNodes::PortType portType,
				     QtNodes::PortIndex portIndex,
				     QVariant const &value,
				     QtNodes::PortRole role = QtNodes::PortRole::Data);
	    
	    virtual bool deleteConnection(QtNodes::ConnectionId const connectionId);
	    
	    virtual bool deleteNode(QtNodes::NodeId const nodeId);
	    
	    /**
	     * Reimplement the function if you want to store/restore the node's
	     * inner state during undo/redo node deletion operations.
	     */
	    virtual QJsonObject saveNode(QtNodes::NodeId const) const;
	    
	    /**
	     * Reimplement the function if you want to support:
	     *
	     *   - graph save/restore operations,
	     *   - undo/redo operations after deleting the node.
	     *
	     * QJsonObject must contain following fields:
	     *
	     *
	     * ```json
	     * {
	     *   id : 5,
	     *   position : { x : 100, y : 200 },
	     *   internal-data {
	     *     "your model specific data here"
	     *   }
	     * }
	     * ```
	     *
	     * The function must do almost exacly the same thing as the normal addNode().
	     * The main difference is in a model-specific `inner-data` processing.
	     */
	    virtual void loadNode(QJsonObject const &);
	    
	    virtual bool loopsEnabled() const { return false; }
	    
	    /**
	     * This method is used to indicate that the given node should be a part
	     * of the given node group.  Node groups are not heirarchal, so
	     * if a node is a member of one group, it is not a member of another.
	     */
	    virtual void setNodeGroup(QtNodes::NodeId const nodeId, QtNodes::GroupId const groupId) { Q_UNUSED(nodeId); Q_UNUSED(groupId); }
	    /**
	     * This method is used to indicate that the given node should no longer
	     * be a part of any node group.
	     */
	    virtual void unsetNodeGroup(QtNodes::NodeId const nodeId) { Q_UNUSED(nodeId); }

	    std::map<QtNodes::GroupId, std::vector<QtNodes::NodeId>> getGroups() const;

	    const NodeJS::core::NodeGraph & getNodeGraph() const;

	Q_SIGNALS:
	    void inPortDataWasSet(QtNodes::NodeId const, QtNodes::PortType const, QtNodes::PortIndex const);

	    
	private:
	    NodeJS::core::NodeGraph & mGraph;

	    /**
	     * These map nodes in the UI to nodes in the underlying graph model.
	     */
	    std::map<QtNodes::NodeId, NodeJS::core::NodeId> mID_toGraph;
	    std::map<NodeJS::core::NodeId, QtNodes::NodeId> mID_fromGraph;
	    std::unique_ptr<std::map<QtNodes::NodeId, QWidget *>> mWidgets;// This is a pointer to fool the const qualifier.
	    QtNodes::NodeId mID_nextNew;
	    NodeEditorWidget & mEditorWidget;
	    const std::map<std::string, NodeEditorWidget::WidgetFactory> & mWidgetFactory;
	};
    }
}

