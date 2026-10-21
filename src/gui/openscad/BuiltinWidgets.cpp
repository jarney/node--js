#include "BuiltinWidgets.hpp"

#include <QtWidgets/QLineEdit>
#include <QIntValidator>
#include <QDoubleValidator>

using namespace NodeJS::core;
using namespace NodeJS::gui::openscad;

void
BuiltinWidgets::registerWidgetFactories(NodeJS::gui::NodeEditorWidget & editor)
{
    editor.registerNodeWidgetFactory("org.ensor.nodejs.openscad.const_int", BuiltinWidgets::widget_factory_const_int);
    editor.registerNodeWidgetFactory("org.ensor.nodejs.openscad.const_string", BuiltinWidgets::widget_factory_const_string);
}

#define _NODEJS_DEFINE_WIDGET_FACTORY(name)				\
    QWidget* BuiltinWidgets::name(NodeJS::gui::NodeEditorWidget & editor, NodeJS::core::Node & node)

_NODEJS_DEFINE_WIDGET_FACTORY(widget_factory_const_int)
{
    QLineEdit *textEdit = new QLineEdit();
    textEdit->setText(QString::fromStdString(node.getData().getValue("value")));
    QIntValidator *validator = new QIntValidator();
    textEdit->setValidator(validator);
    QObject::connect(textEdit, &QLineEdit::textChanged, [&node, textEdit]() {
        if (textEdit->hasAcceptableInput()) {
            node.getData().setValue("value", textEdit->text().toStdString());
        }
    });
    return textEdit;
}

_NODEJS_DEFINE_WIDGET_FACTORY(widget_factory_const_string)
{
    QLineEdit *textEdit = new QLineEdit();
    textEdit->setText(QString::fromStdString(node.getData().getValue("value")));
    QObject::connect(textEdit, &QLineEdit::textChanged, [&node, textEdit]() {
        node.getData().setValue("value", textEdit->text().toStdString());
    });

    return textEdit;
}

