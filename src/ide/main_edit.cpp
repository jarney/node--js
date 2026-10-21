#include <Qsci/qsciscintilla.h>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>

#include <QtNodes/GraphicsView>
#include <QtNodes/NodeData>

#include <QtGui/QScreen>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QTabWidget>

#include <QtGui/QScreen>
#include <iostream>
#include <fstream>

#include <node--js/NodeModule.hpp>
#include <node--js/Natives.hpp>
#include <node--js/Processor.hpp>
#include <node--js/xml/SerializerXML.hpp>
#include <node--js/xml/ModuleLoaderNodeJSPath.hpp>
#include <node--js/engines/openscad/Builtins.hpp>

#include <node--js/gui/NodeEditorWidget.hpp>
#include <node--js/gui/openscad/BuiltinWidgets.hpp>
#include <node--js/gui/core/BuiltinWidgets.hpp>

using QtNodes::GraphicsView;

using namespace NodeJS::core;
using namespace NodeJS::xml;
using namespace NodeJS::gui;

void
write_module_to_source_tab(const NodeModule & nodeModule, QsciScintilla *qsci_source);

void
write_module_to_output_tab(const NodeModule & nodeModule, QsciScintilla *qsci_output, std::string graphId);

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    if (argc != 3) {
	fprintf(stderr, "Usage: edit filename graph-id\n");
	return 1;
    }
    SerializerErrorReporterStream err(std::cerr);
    
    ModuleLoader loader;
    std::unique_ptr<ModuleLoaderNodeJSPath> plugin = std::make_unique<ModuleLoaderNodeJSPath>();
    //plugin->setNODEJS_PATH("../submodules/node--js/test-data/xml/Serializer:.");
    loader.setPlugin(std::move(plugin));
    std::string moduleName(argv[1]);
    
    NodeModule *loaded = loader.loadModule(moduleName, err);
    if (loaded == nullptr) {
	fprintf(stderr, "Could not find module %s in NODEJS_PATH\n", argv[1]);
	return 2;
    }
    NodeModule & nodeModule = *loaded;

    // Register builtins...
    NodeJS::gui::NodeEditorWidget::initializeStyles();

    QWidget mainWidget;

    auto menuBar = new QMenuBar();
    QMenu *menu = menuBar->addMenu("File");

    auto saveAction = menu->addAction("Save Scene");
    saveAction->setShortcut(QKeySequence::Save);

    auto loadAction = menu->addAction("Load Scene");
    loadAction->setShortcut(QKeySequence::Open);

    auto groupAction = menu->addAction("Create Group");
    groupAction->setShortcut(QKeySequence::Close);

    QVBoxLayout *l = new QVBoxLayout(&mainWidget);
    l->setContentsMargins(0, 0, 0, 0);
    l->setSpacing(0);
    l->addWidget(menuBar);

    auto qtab = new QTabWidget(&mainWidget);
    auto qtabLayout = new QVBoxLayout(qtab);
    l->addWidget(qtab);

    std::string graphId(argv[2]);
    NodeEditorWidget *jw = new NodeEditorWidget(nodeModule);
    NodeJS::gui::core::BuiltinWidgets::registerWidgetFactories(*jw);
    NodeJS::gui::openscad::BuiltinWidgets::registerWidgetFactories(*jw);
    
    jw->editGraph(graphId);
    
    qtab->addTab(jw, "Nodes");

    auto qsci_source = new QsciScintilla(qtab);
    qtab->addTab(qsci_source, "Source");

    auto qsci_output = new QsciScintilla(qtab);
    qtab->addTab(qsci_output, "Output");

    QObject::connect(qtab, &QTabWidget::currentChanged, [&nodeModule, &qsci_source, &qsci_output, &graphId]() {
	// Just do a vanilla output of the raw package's xml:
	write_module_to_source_tab(nodeModule, qsci_source);

	// Process the package and show the result.
	write_module_to_output_tab(nodeModule, qsci_output, graphId);
	
    });

    QObject::connect(saveAction, &QAction::triggered, [&nodeModule]() {
	fprintf(stderr, "Writing model back to file %s\n", nodeModule.getPackage().c_str());
        const SerializerXML & ser = SerializerXML::instance();
        SerializerErrorReporterStream err(std::cerr);
	std::ofstream output_stream(nodeModule.getPackage() + ".xml", std::ios::binary | std::ios::trunc);
	ser.write(nodeModule.getPackage(), nodeModule, output_stream, err);
	output_stream.close();
    });

    // This is a hot mess, but fortunately we should not really
    // need to do this in the final product.
    QObject::connect(loadAction, &QAction::triggered, [qtab, &nodeModule, argv]() {
#if 0
	qtab->removeTab(0);
	const NodeProgramSerializer & serializer = NodeProgramSerializerJSON::instance();
	std::ifstream input(argv[1]);
	serializer.read(program, input);
	NodeEditorWidget *jw = new NodeEditorWidget(program);
	qtab->insertTab(0, jw, "Nodes-");
	qtab->setTabVisible(0, true);
#endif
    });

    QObject::connect(groupAction, &QAction::triggered, [jw]() {
	std::vector<QtNodes::NodeGraphicsObject*> groupNodes = jw->selectedNodes();
	jw->createGroup(groupNodes, QString("Some Group"));
    });
    
    mainWidget.setWindowTitle("[*]Data Flow: simplest calculator");
    mainWidget.resize(800, 600);
    // Center window.
    mainWidget.move(QApplication::primaryScreen()->availableGeometry().center()
                    - mainWidget.rect().center());
    mainWidget.showNormal();

    return app.exec();
}

void
write_module_to_source_tab(const NodeModule & nodeModule, QsciScintilla *qsci_source)
{
        const SerializerXML & ser = SerializerXML::instance();
        SerializerErrorReporterStream err(std::cerr);
	std::ostringstream output_stream;
	ser.write(nodeModule.getPackage(), nodeModule, output_stream, err);
	qsci_source->setText(QString::fromStdString(output_stream.str()));

}
void
write_module_to_output_tab(const NodeModule & nodeModule, QsciScintilla *qsci_output, std::string graphId)
{
    Processor processor;
    
    ConnectionData input;
    ConnectionData output;
    NodeGraph *graph = nodeModule.getGraph(graphId);
    if (!graph) {
	std::string output_str = std::string("Graph '") + graphId + std::string("' not found in module '") + nodeModule.getPackage().c_str() + std::string("'\n");
	qsci_output->setText(QString::fromStdString(output_str));
    }
    
    processor.setNativeImpl(NodeProcessor_string::ID, std::make_unique<NodeProcessor_string>());
    processor.setNativeImpl(NodeProcessor_concatenate::ID, std::make_unique<NodeProcessor_concatenate>());
    processor.setNativeImpl(NodeProcessor_input::ID, std::make_unique<NodeProcessor_input>());
    processor.setNativeImpl(NodeProcessor_output::ID, std::make_unique<NodeProcessor_output>());

    NodeJS::openscad::Builtins::registerProcessors(processor);

    processor.processGraph(*graph, input, output);

    std::string assignments_str = output.getValue("assignments");
    std::string function_definitions_str = output.getValue("function-definitions");
    std::string module_definitions_str = output.getValue("module-definitions");
    std::string module_instantiations_str = output.getValue("module-instantiations");

    std::string output_str = function_definitions_str + module_definitions_str + assignments_str + module_instantiations_str;
    
    qsci_output->setText(QString::fromStdString(output_str));

}

