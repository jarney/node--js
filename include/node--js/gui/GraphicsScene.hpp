#pragma once

#include <QtNodes/BasicGraphicsScene>
#include <QtNodes/internal/ConnectionGraphicsObject.hpp>
#include <QtNodes/internal/Export.hpp>
#include <QtNodes/internal/NodeConnectionInteraction.hpp>

#include <node--js/NodeGraph.hpp>
#include <node--js/gui/GraphModelAdapter.hpp>

namespace NodeJS {
    namespace gui {

/**
 * @brief An advanced scene working with data-propagating graphs.
 *
 * The class represents a scene that existed in v2.x but built wit the
 * new model-view approach in mind.
 */
class NODE_EDITOR_PUBLIC GraphicsScene : public QtNodes::BasicGraphicsScene
{
    Q_OBJECT
public:
    GraphicsScene(
	std::unique_ptr<NodeJS::gui::GraphModelAdapter> graphModel,
	QObject *parent = nullptr
	);
    ~GraphicsScene() = default;

public:
    std::vector<QtNodes::NodeId> selectedNodes() const;
    QMenu *createSceneMenu(QPointF const scenePos) override;
    void updateConnectionGraphics(const std::unordered_set<QtNodes::ConnectionId> &connections, bool state);

    QMenu *createGroupMenu(QPointF const scenePos, QtNodes::GroupGraphicsObject *groupGo);

public Q_SLOTS:
#if 0
    bool save() const;
    bool load();
#endif

Q_SIGNALS:
    void sceneLoaded();

private:
    std::unique_ptr<NodeJS::gui::GraphModelAdapter> mGraphModel;
};

    } // End gui
} // End NodeJS
