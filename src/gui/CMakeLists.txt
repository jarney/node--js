  find_package(Qt6 COMPONENTS Core Core5Compat Widgets Multimedia OpenGL OpenGLWidgets Concurrent Network Svg ${QT_TEST_IF_ANY} REQUIRED QUIET)
    message(STATUS "Qt6: ${Qt6_VERSION}")

    if (Qt6_POSITION_INDEPENDENT_CODE)
      set_target_properties(OpenSCADLibInternal svg OpenSCADExe PROPERTIES
	POSITION_INDEPENDENT_CODE ON
      )
    endif()

    find_package(Qt6QScintilla 2.8.0 REQUIRED)
    message(STATUS "QScintilla: ${QT6QSCINTILLA_VERSION_STRING}")

    if(ENABLE_QTDBUS)
      find_package(Qt6DBus QUIET)
      if (Qt6DBus_FOUND)
        message(STATUS "DBus input driver enabled")
        target_compile_definitions(OpenSCADLibInternal PUBLIC ENABLE_DBUS)
        set(INPUT_DRIVER_DBUS_SOURCES src/gui/input/DBusInputDriver.cc)
        qt6_add_dbus_interface(INPUT_DRIVER_DBUS_SOURCES org.openscad.OpenSCAD.xml openscad_interface)
        qt6_add_dbus_adaptor(INPUT_DRIVER_DBUS_SOURCES org.openscad.OpenSCAD.xml gui/input/DBusInputDriver.h DBusInputDriver openscad_adaptor)
      else()
        message(STATUS "DBus input driver disabled as the QtDBus module could not be found.")
      endif()
    else()
      message(STATUS "DBus input driver disabled per user request.")
    endif()


set(NODEJS_QT_GUI_CPP_SOURCE_FILES
    BreadcrumbsWidget.cpp
    GraphModelAdapter.cpp
    GraphicsScene.cpp
    NodeEditorWidget.cpp
    core/BuiltinWidgets.cpp
    openscad/BuiltinWidgets.cpp
)

set(NODEJS_QT_GUI_HPP_HEADER_FILES
    ${PROJECT_SOURCE_DIR}/include/node--js/gui/BreadcrumbsWidget.hpp
    ${PROJECT_SOURCE_DIR}/include/node--js/gui/GraphModelAdapter.hpp
    ${PROJECT_SOURCE_DIR}/include/node--js/gui/GraphicsScene.hpp
    ${PROJECT_SOURCE_DIR}/include/node--js/gui/NodeEditorWidget.hpp
    ${PROJECT_SOURCE_DIR}/include/node--js/gui/core/BuiltinWidgets.hpp
    ${PROJECT_SOURCE_DIR}/include/node--js/gui/openscad/BuiltinWidgets.hpp
)

# If we want to give the option to build a static library,
# set BUILD_SHARED_LIBS option to OFF
add_library(NodeJSGuiQt
  ${NODEJS_QT_GUI_CPP_SOURCE_FILES}
  ${NODEJS_QT_GUI_HPP_HEADER_FILES}
)

add_library(NodeJS::Qt ALIAS NodeJSGuiQt)

target_include_directories(NodeJSGuiQt
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
  PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
)

target_link_libraries(NodeJSGuiQt
  PUBLIC
)

#target_compile_definitions(QtNodes PUBLIC NODE_EDITOR_SHARED)
#target_compile_options(NodeJSGuiQt PUBLIC -DNODE_EDITOR_SHARED)
#target_compile_options(QtNodes PUBLIC -DNODE_EDITOR_SHARED)

find_package(LibXml2 2.9 REQUIRED QUIET)
message(STATUS "LibXml2: ${LIBXML2_VERSION_STRING}")
FIND_PACKAGE( Boost 1.40 COMPONENTS program_options REQUIRED )

target_link_libraries(NodeJSGuiQt PUBLIC NodeJS::NodeJS)
target_link_libraries(NodeJSGuiQt PUBLIC QtNodes)
target_link_libraries(NodeJSGuiQt PUBLIC
      Qt6::Core Qt6::Core5Compat Qt6::Widgets Qt6::Multimedia Qt6::OpenGL Qt6::OpenGLWidgets Qt6::Concurrent Qt6::Network Qt6::Svg
      ${QT6QSCINTILLA_LIBRARY}
      )

if(NOT "${CMAKE_CXX_SIMULATE_ID}" STREQUAL "MSVC")
  # Clang-Cl on MSVC identifies as "Clang" but behaves more like MSVC:
  target_compile_options(NodeJSGuiQt
    PRIVATE
      $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra>
  )
endif()

if(QT_NODES_DEVELOPER_DEFAULTS)
  target_compile_features(NodeJSGuiQt PUBLIC cxx_std_14)
  set_target_properties(NodeJSGuiQt PROPERTIES CXX_EXTENSIONS OFF)
endif()


set_target_properties(NodeJSGuiQt
  PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Some magic QT stuff for widgets and such.
set_target_properties(NodeJSGuiQt PROPERTIES
  AUTOMOC ON
  AUTOUIC ON
  AUTORCC ON
)
