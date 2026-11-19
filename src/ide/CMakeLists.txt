find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
project(calculatorform LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(NODEJS_IDE_SOURCE_FILES
    main_edit.cpp
    )

set(NODEJS_IDE_HEADER_FILES )

add_executable(NodeJSIDE ${NODEJS_IDE_SOURCE_FILES}
  grapheditpanel.h grapheditpanel.cpp grapheditpanel.ui)
set_target_properties(NodeJSIDE PROPERTIES
  OUTPUT_NAME "ide"
)
target_link_libraries(NodeJSIDE NodeJSGuiQt)

#project(calculatorform LANGUAGES CXX)
#
#qt_add_executable(calculatorform main.cpp mainwindow2.ui)
#
#target_link_libraries(calculatorform  PUBLIC
#				      Qt::Core
#				      Qt::Gui
#				      Qt::Widgets
#)
