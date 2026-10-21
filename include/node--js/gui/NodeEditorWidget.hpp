#pragma once

#include <node--js/gui/BreadcrumbsWidget.hpp>
#include <node--js/NodeModule.hpp>

namespace QtNodes {
    class NodeGraphicsObject;
}

namespace NodeJS {
    namespace gui {

class NodeEditorWidget : public QWidget {
public:
    typedef std::function<QWidget*(NodeEditorWidget & editor, NodeJS::core::Node & node)> WidgetFactory;

    NodeEditorWidget(NodeJS::core::NodeModule & program);
    ~NodeEditorWidget();
    void editGraph(std::string editGraph);
    void registerNodeWidgetFactory(std::string nodeTypeId, WidgetFactory factory);

    std::vector<QtNodes::NodeGraphicsObject*> selectedNodes();
    void createGroup(std::vector<QtNodes::NodeGraphicsObject*> & groupNodes, QString name);
    
    static void initializeStyles();
private:
    /**
     * This connects the program to the editor
     * widget so it can navigate on its behalf.
     */
    void prepareProgram();
    
    NodeJS::core::NodeModule &_program;
    std::unique_ptr<QVBoxLayout> layout;
    BreadcrumbsWidget *_jbreadcrumbs;
    std::map<std::string, WidgetFactory> mWidgetFactory;
};


    } // End gui
} // End NodeJS
