#include <QtNodes/internal/ConnectionGraphicsObject.hpp>
#include <QtNodes/GraphicsView>
#include <QtNodes/internal/NodeGraphicsObject.hpp>
#include <QtNodes/UndoCommands>

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QGraphicsSceneMoveEvent>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidgetAction>
#include <QtWidgets/QMenu>

#include <QtCore/QBuffer>
#include <QtCore/QByteArray>
#include <QtCore/QDataStream>
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonParseError>
#include <QtCore/QJsonValue>
#include <QtCore/QUuid>
#include <QtCore/QtGlobal>

#include <stdexcept>
#include <utility>
#include <vector>

#include <node--js/gui/GraphicsScene.hpp>
#include <node--js/NodeModule.hpp>
#include <node--js/NodeType.hpp>
#include <node--js/Metadata.hpp>

using namespace NodeJS::gui;
using namespace NodeJS::core;

static QtNodes::GroupId jsonValueToGroupId(QJsonValue const &value)
{
    if (value.isDouble()) {
        return static_cast<QtNodes::GroupId>(value.toInt());
    }

    if (value.isString()) {
        auto const textValue = value.toString();

        bool ok = false;
        auto const numericValue = textValue.toULongLong(&ok, 10);
        if (ok) {
            return static_cast<QtNodes::GroupId>(numericValue);
        }

        QUuid uuidValue(textValue);
        if (!uuidValue.isNull()) {
            auto const bytes = uuidValue.toRfc4122();
            if (bytes.size() >= static_cast<int>(sizeof(quint32))) {
                QDataStream stream(bytes);
                quint32 value32 = 0U;
                stream >> value32;
                return static_cast<QtNodes::GroupId>(value32);
            }
        }
    }

    return QtNodes::InvalidGroupId;
}

GraphicsScene::GraphicsScene(
    std::unique_ptr<GraphModelAdapter> aGraphModel,
    QObject *parent
    )
    : BasicGraphicsScene(*aGraphModel.get(), parent)
    , mGraphModel(std::move(aGraphModel))
{
    auto groupMap = mGraphModel->getGroups();
    for (const auto & groupIt : groupMap) {
	std::vector<QtNodes::NodeGraphicsObject*> nodeGraphicsObjects;
	for (const auto & nodeId : groupIt.second) {
	    nodeGraphicsObjects.push_back(nodeGraphicsObject(nodeId));
	}
	createGroup(nodeGraphicsObjects, QString::number(groupIt.first), groupIt.first);
    }
    
    connect(mGraphModel.get(),
            &GraphModelAdapter::inPortDataWasSet,
            [this](QtNodes::NodeId const nodeId, QtNodes::PortType const, QtNodes::PortIndex const) { onNodeUpdated(nodeId); });
}

// TODO constructor for an empyt scene?

std::vector<QtNodes::NodeId> GraphicsScene::selectedNodes() const
{
    QList<QGraphicsItem *> graphicsItems = selectedItems();

    std::vector<QtNodes::NodeId> result;
    result.reserve(graphicsItems.size());

    for (QGraphicsItem *item : graphicsItems) {
        auto ngo = qgraphicsitem_cast<QtNodes::NodeGraphicsObject *>(item);

        if (ngo != nullptr) {
            result.push_back(ngo->nodeId());
        }
    }

    return result;
}

static std::map<std::string, std::unique_ptr<QIcon>> _icons;
static const QIcon & iconCache(std::string name)
{
    const auto & it = _icons.find(name);
    if (it == _icons.end()) {
	auto icon = std::make_unique<QIcon>(QString::fromStdString(name));
	const QIcon & icon_ref = *icon.get();
	_icons.insert(std::make_pair(name, std::move(icon)));
	return icon_ref;
    }
    else {
	return *(it->second.get());
    }
}

static void
addNodeTypesTo(QTreeWidget *treeParent, QTreeWidgetItem *itemParent, const std::map<std::string, std::unique_ptr<NodeType>> & nodeTypes)
{
    std::map<std::string, QTreeWidgetItem *> categories;
    for (const auto & nodeTypeIt : nodeTypes) {
	const std::unique_ptr<NodeType> & nodeType = nodeTypeIt.second;
	const ConnectionData & nodeTypeMetadata = nodeType->getMetadata().getMetadata("http://jarney.github.io/nodejs/editor");

	if (nodeTypeMetadata.hasValue("category")) {
	    std::string categoryStr = nodeTypeMetadata.getValue("category");
	    if (categories.find(categoryStr) == categories.end()) {
		QTreeWidgetItem *categoryItem;
		if (treeParent) {
		    categoryItem = new QTreeWidgetItem(treeParent);
		}
		else if (itemParent) {
		    categoryItem = new QTreeWidgetItem(itemParent);
		}
		else {
		    continue;
		}
		
		categoryItem->setFlags(categoryItem->flags() & ~Qt::ItemIsSelectable);
		categoryItem->setExpanded(false); // Expand by default because there are a lot of nodes.
		categoryItem->setText(0, QString::fromStdString(categoryStr));
		categories[categoryStr] = categoryItem;
	    }
	}
	
    }

    // Locally defined nodes go first in the menu.
    for (const auto & nodeTypeIt : nodeTypes) {
	const std::unique_ptr<NodeType> & nodeType = nodeTypeIt.second;
	const ConnectionData & nodeTypeMetadata = nodeType->getMetadata().getMetadata("http://jarney.github.io/nodejs/editor");
	
        QTreeWidgetItem *nodeTypeItem;
	if (nodeTypeMetadata.hasValue("category")) {
	    QTreeWidgetItem *itemParent = categories[nodeTypeMetadata.getValue("category")];
	    nodeTypeItem = new QTreeWidgetItem(itemParent);
	}
	else {
	    if (treeParent) {
		nodeTypeItem = new QTreeWidgetItem(treeParent);
	    }
	    else if (itemParent) {
		nodeTypeItem = new QTreeWidgetItem(itemParent);
	    }
	}
	if (nodeTypeMetadata.hasValue("name")) {
	    nodeTypeItem->setText(0, QString::fromStdString(nodeTypeMetadata.getValue("name")));
	}
	else {
	    nodeTypeItem->setText(0, QString::fromStdString(nodeType->getLocalId()));
	}
	if (nodeTypeMetadata.hasValue("description")) {
	    nodeTypeItem->setToolTip(0, QString::fromStdString(nodeTypeMetadata.getValue("description")));
	}
        nodeTypeItem->setData(0, Qt::UserRole, QVariant(QString::fromStdString(nodeType->getFullyQualifiedId())));
	if (nodeTypeMetadata.hasValue("icon")) {
	    nodeTypeItem->setIcon(0, iconCache(nodeTypeMetadata.getValue("icon")));
	}
    }
}

QMenu *GraphicsScene::createSceneMenu(QPointF const scenePos)
{
    QMenu *modelMenu = new QMenu();

    // Add filterbox to the context menu
    auto *txtBox = new QLineEdit(modelMenu);
    txtBox->setPlaceholderText(QStringLiteral("Filter"));
    txtBox->setClearButtonEnabled(true);

    auto *txtBoxAction = new QWidgetAction(modelMenu);
    txtBoxAction->setDefaultWidget(txtBox);

    // 1.
    modelMenu->addAction(txtBoxAction);

    // Add result treeview to the context menu
    QTreeWidget *treeView = new QTreeWidget(modelMenu);
    treeView->header()->close();

    auto *treeViewAction = new QWidgetAction(modelMenu);
    treeViewAction->setDefaultWidget(treeView);

    // 2.
    modelMenu->addAction(treeViewAction);

    // The job here is to inspect the graph/module
    // and pull out all nodes we could possibly use
    // organized by module and category.  User is allowed to
    // search by node name.
    const NodeGraph & graph = mGraphModel->getNodeGraph();
    const NodeModule & nodeModule = graph.getModule();

    addNodeTypesTo(treeView, nullptr, nodeModule.getNodeTypes());

    std::vector<std::string> importedModuleList = nodeModule.getImportModuleList();
    for (const auto & importedModuleId : importedModuleList) {
        const NodeModule *importedModule = nodeModule.getImportModule(importedModuleId);
	const ConnectionData & moduleMetadata = importedModule->getMetadata().getMetadata("http://jarney.github.io/nodejs/editor");

	// Imported module
        auto importedModuleItem = new QTreeWidgetItem(treeView);
	importedModuleItem->setExpanded(true); // We want to see all of the modules.
	importedModuleItem->setFlags(importedModuleItem->flags() & ~Qt::ItemIsSelectable);
	if (moduleMetadata.hasValue("name")) {
	    importedModuleItem->setText(0, QString::fromStdString(moduleMetadata.getValue("name")));
	}
	else {
	    importedModuleItem->setText(0, QString::fromStdString(importedModuleId));
	}
	if (moduleMetadata.hasValue("description")) {
	    importedModuleItem->setToolTip(0, QString::fromStdString(moduleMetadata.getValue("description")));
	}
	//categoryItem->setIcon(0, iconCache(categoryObj->getIcon())); TODO : Get icons from our module's metadata, probably.

	addNodeTypesTo(nullptr, importedModuleItem, importedModule->getNodeTypes());
    }
    
//    treeView->expandAll();

    connect(treeView,
            &QTreeWidget::itemClicked,
            [this, modelMenu, scenePos](QTreeWidgetItem *item, int) {
                if (!(item->flags() & (Qt::ItemIsSelectable))) {
                    return;
                }

		// This is the key element.  We need to
		// create the command with the name of the node
		// but the menu contains (will contain) the description.
		QVariant modelType = item->data(0, Qt::UserRole);
                this->undoStack().push(new QtNodes::CreateCommand(this, modelType.toString(), scenePos));

                modelMenu->close();
            });

    //Setup filtering
    connect(txtBox, &QLineEdit::textChanged, [treeView](const QString &text) {
        QTreeWidgetItemIterator categoryIt(treeView, QTreeWidgetItemIterator::HasChildren);
        while (*categoryIt)
            (*categoryIt++)->setHidden(true);
        QTreeWidgetItemIterator it(treeView, QTreeWidgetItemIterator::NoChildren);
        while (*it) {
            auto modelName = (*it)->text(0);
            const bool match = ((*it)->text(0).contains(text, Qt::CaseInsensitive)) ||
		((*it)->toolTip(0).contains(text, Qt::CaseInsensitive));
            (*it)->setHidden(!match);
            if (match) {
                QTreeWidgetItem *parent = (*it)->parent();
                while (parent) {
                    parent->setHidden(false);
                    parent = parent->parent();
                }
            }
            ++it;
        }
    });

    // make sure the text box gets focus so the user doesn't have to click on it
    txtBox->setFocus();

    // QMenu's instance auto-destruction
    modelMenu->setAttribute(Qt::WA_DeleteOnClose);

    return modelMenu;
}

#if 0
bool GraphicsScene::save() const
{
    QString fileName = QFileDialog::getSaveFileName(nullptr,
                                                    tr("Open Flow Scene"),
                                                    QDir::homePath(),
                                                    tr("Flow Scene Files (*.flow)"));
    if (!fileName.isEmpty()) {
        if (!fileName.endsWith("flow", Qt::CaseInsensitive))
            fileName += ".flow";

        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly)) {
            QJsonObject sceneJson = _graphModel.save();

            QJsonArray groupsJsonArray;
            for (auto const &[groupId, groupPtr] : groups()) {
                if (!groupPtr)
                    continue;

                QJsonObject groupJson;
                groupJson["id"] = static_cast<qint64>(groupId);
                groupJson["name"] = groupPtr->name();

                QJsonArray nodeIdsJson;
                for (QtNodes::NodeId const nodeId : groupPtr->nodeIDs()) {
                    nodeIdsJson.append(static_cast<qint64>(nodeId));
                }
                groupJson["nodes"] = nodeIdsJson;
                groupJson["locked"] = groupPtr->groupGraphicsObject().locked();

                groupsJsonArray.append(groupJson);
            }

            if (!groupsJsonArray.isEmpty()) {
                sceneJson["groups"] = groupsJsonArray;
            }

            file.write(QJsonDocument(sceneJson).toJson());
            return true;
        }
    }
    return false;
}

bool GraphicsScene::load()
{
    QString fileName = QFileDialog::getOpenFileName(nullptr,
                                                    tr("Open Flow Scene"),
                                                    QDir::homePath(),
                                                    tr("Flow Scene Files (*.flow)"));

    if (!QFileInfo::exists(fileName))
        return false;

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly))
        return false;

    clearScene();

    QByteArray const wholeFile = file.readAll();

    QJsonParseError parseError{};
    QJsonDocument const sceneDocument = QJsonDocument::fromJson(wholeFile, &parseError);
    if (parseError.error != QJsonParseError::NoError || !sceneDocument.isObject())
        return false;

    QJsonObject const sceneJson = sceneDocument.object();

    _graphModel.load(sceneJson);

    if (sceneJson.contains("groups")) {
        QJsonArray const groupsJsonArray = sceneJson["groups"].toArray();

        for (QJsonValue groupValue : groupsJsonArray) {
            QJsonObject const groupObject = groupValue.toObject();

            QJsonArray const nodeIdsJson = groupObject["nodes"].toArray();
            std::vector<QtNodes::NodeGraphicsObject *> groupNodes;
            groupNodes.reserve(nodeIdsJson.size());

            for (QJsonValue idValue : nodeIdsJson) {
                QtNodes::NodeId const nodeId = static_cast<QtNodes::NodeId>(idValue.toInt());
                if (auto *nodeObject = nodeGraphicsObject(nodeId)) {
                    groupNodes.push_back(nodeObject);
                }
            }

            if (groupNodes.empty())
                continue;

            QString const groupName = groupObject["name"].toString();
	    QtNodes::GroupId const groupId = jsonValueToGroupId(groupObject["id"]);

            auto const groupWeak = createGroup(groupNodes, groupName, groupId);
            if (auto group = groupWeak.lock()) {
                bool const locked = groupObject["locked"].toBool(true);
                group->groupGraphicsObject().lock(locked);
            }
        }
    }

    Q_EMIT sceneLoaded();

    return true;
}
#endif

void GraphicsScene::updateConnectionGraphics(
    const std::unordered_set<QtNodes::ConnectionId> &connections, bool state)
{
    for (auto const &c : connections) {
        if (auto *cgo = connectionGraphicsObject(c)) {
            cgo->connectionState().setFrozen(state);
            cgo->update();
        }
    }
}


QMenu *
GraphicsScene::createGroupMenu(QPointF const scenePos, QtNodes::GroupGraphicsObject *groupGo)
{ return nullptr; }
