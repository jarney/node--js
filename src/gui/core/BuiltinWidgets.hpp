#pragma once

#include <QtCore/QObject>
#include <QtWidgets/QLabel>

#include "node--js/NodeType.hpp"
#include "node--js/Node.hpp"

#include "nodes/gui/NodeEditorWidget.hpp"

namespace NodeJS {
    namespace gui {
	namespace core {
	    class BuiltinWidgets {
	    public:
		BuiltinWidgets() = delete;
		~BuiltinWidgets() = delete;
		static void registerWidgetFactories(NodeJS::gui::NodeEditorWidget & editor);
		
	    private:
#define _NODEJS_DECLARE_WIDGET_FACTORY(name)				\
		static QWidget* name(NodeJS::gui::NodeEditorWidget & editor, NodeJS::core::Node & node)
		
		_NODEJS_DECLARE_WIDGET_FACTORY(widget_factory_string);
		
#undef _NODEJS_DECLARE_WIDGET_FACTORY
	    };
	}
    }
}

