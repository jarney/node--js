#include <QtNodes/GraphicsView>
#include <QtNodes/ConnectionStyle>

#include <node--js/Node.hpp>
#include <node--js/NodeGraph.hpp>
#include <node--js/NodeModule.hpp>

#include <node--js/gui/NodeEditorWidget.hpp>
#include <node--js/gui/GraphicsScene.hpp>

using namespace NodeJS::gui;
using namespace NodeJS::core;

NodeEditorWidget::NodeEditorWidget(NodeModule & program)
    : _program(program)
{
    layout = std::make_unique<QVBoxLayout>(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    _jbreadcrumbs = new BreadcrumbsWidget();
    layout->addWidget(_jbreadcrumbs);
}

void
NodeEditorWidget::registerNodeWidgetFactory(std::string nodeTypeId, WidgetFactory factory)
{
    mWidgetFactory[nodeTypeId] = factory;
}

NodeEditorWidget::~NodeEditorWidget()
{}


void
NodeEditorWidget::editGraph(std::string editGraph)
{
    NodeGraph *graph = _program.getGraph(editGraph);
    if (graph == nullptr) {
	graph = _program.addGraph(editGraph);
    }
    std::unique_ptr<GraphModelAdapter> model = std::make_unique<GraphModelAdapter>(*graph, *this, mWidgetFactory);
    auto scene = new GraphicsScene(std::move(model));

    // Load up the groups.
    
    auto view = new QtNodes::GraphicsView(scene);
    view->centerScene();
    _jbreadcrumbs->addPage(view);
}

std::vector<QtNodes::NodeGraphicsObject*>
NodeEditorWidget::selectedNodes()
{
    QtNodes::GraphicsView *view = (QtNodes::GraphicsView *)_jbreadcrumbs->getPage();
    QtNodes::BasicGraphicsScene *scene = view->getNodeScene();
    return scene->selectedNodes();
}

void
NodeEditorWidget::createGroup(std::vector<QtNodes::NodeGraphicsObject*> & groupNodes, QString name)
{
    QtNodes::GraphicsView *view = (QtNodes::GraphicsView *)_jbreadcrumbs->getPage();
    QtNodes::BasicGraphicsScene *scene = view->getNodeScene();
    scene->createGroup(groupNodes, name);
}


void
NodeEditorWidget::initializeStyles()
{
    QtNodes::ConnectionStyle::setConnectionStyle(
        R"(
  {
    "ConnectionStyle": {
      "ConstructionColor": "gray",
      "NormalColor": "black",
      "SelectedColor": "gray",
      "SelectedHaloColor": "deepskyblue",
      "HoveredColor": "deepskyblue",

      "LineWidth": 3.0,
      "ConstructionLineWidth": 2.0,
      "PointDiameter": 10.0,

      "UseDataDefinedColors": true
    }
  }
  )");
}

