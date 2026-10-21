

set(NODEJS_IDE_SOURCE_FILES
    main_edit.cpp
    )

set(NODEJS_IDE_HEADER_FILES )

add_executable(NodeJSIDE ${NODEJS_IDE_SOURCE_FILES})
set_target_properties(NodeJSIDE PROPERTIES
  OUTPUT_NAME "ide"
)
target_link_libraries(NodeJSIDE NodeJSGuiQt)
