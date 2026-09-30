cmake_minimum_required(VERSION 3.11)

cmake_policy(SET CMP0068 NEW) # new in 3.9. The NEW behavior of this policy is to ignore the RPATH settings for install_name on macOS.


project(NodeJS CXX)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES  ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_ENABLE_CODE_COVERAGE ON)

# Enable AUTOMOC globally for all targets (needed for examples with Q_OBJECT)
#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)

get_directory_property(_has_parent PARENT_DIRECTORY)
if(_has_parent)
  set(is_root_project OFF)
else()
  set(is_root_project ON)
endif()

set(NODEJS_DEVELOPER_DEFAULTS "${is_root_project}" CACHE BOOL "Turns on default settings for development of NodeJS")

option(BUILD_TESTING "Build tests" ON)
option(BUILD_PYTHON "Python Module" OFF)
option(BUILD_EXAMPLES "Build Examples" ON)
option(BUILD_DOCS "Build Documentation" OFF)
option(BUILD_SHARED_LIBS "Build as shared library" ON)
option(BUILD_DEBUG_POSTFIX_D "Append d suffix to debug libraries" OFF)
option(BUILD_CODE_COVERAGE "Built code-coverage" "${NODEJS_DEVELOPER_DEFAULTS}")

if(QT_NODES_DEVELOPER_DEFAULTS)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
endif()

if(BUILD_DEBUG_POSTFIX_D)
  set(CMAKE_DEBUG_POSTFIX "d")
  set(CMAKE_RELEASE_POSTFIX "")
  set(CMAKE_RELWITHDEBINFO_POSTFIX "rd")
  set(CMAKE_MINSIZEREL_POSTFIX "s")
endif()


set(CPP_SOURCE_FILES
  src/main.cpp
  src/DataType.cpp
  src/NodeType.cpp
  src/NodePort.cpp
  src/NodeModule.cpp
  src/ConnectionData.cpp
  src/SerializerError.cpp
  src/xml/Serializer.cpp
  src/xml/XmlNodeWrapper.cpp
)

set(HPP_HEADER_FILES
    include/node--js/main.hpp
    include/node--js/DataType.hpp
    include/node--js/NodeType.hpp
    include/node--js/NodePort.hpp
    include/node--js/NodeModule.hpp
    include/node--js/ConnectionData.hpp
    include/node--js/Serializer.hpp
    include/node--js/xml/Serializer.hpp
    include/node--js/xml/XmlNodeWrapper.hpp
)

# If we want to give the option to build a static library,
# set BUILD_SHARED_LIBS option to OFF
add_library(NodeJS
  ${CPP_SOURCE_FILES}
  ${HPP_HEADER_FILES}
)

add_library(NodeJS::NodeJS ALIAS NodeJS)

target_include_directories(NodeJS
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
  PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
)

target_link_libraries(NodeJS
  PUBLIC
)

#target_compile_definitions(NodeJS )

if(MSVC)
  string(REGEX REPLACE "/W[0-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif()

#target_compile_options(NodeJS )

find_package(LibXml2 2.9 REQUIRED QUIET)
message(STATUS "LibXml2: ${LIBXML2_VERSION_STRING}")
target_link_libraries(NodeJS PUBLIC LibXml2::LibXml2)


if(NOT "${CMAKE_CXX_SIMULATE_ID}" STREQUAL "MSVC")
  # Clang-Cl on MSVC identifies as "Clang" but behaves more like MSVC:
  target_compile_options(NodeJS
    PRIVATE
      $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra>
  )
endif()

if(QT_NODES_DEVELOPER_DEFAULTS)
  target_compile_features(NodeJS PUBLIC cxx_std_14)
  set_target_properties(NodeJS PROPERTIES CXX_EXTENSIONS OFF)
endif()


set_target_properties(NodeJS
  PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

###########
# Examples
##
if(BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

##################
# Doxygen/documentation
##
if(BUILD_DOCS)
  message("Enabling doxygen output")
  add_subdirectory(doc)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-unused-function -fno-exceptions")

if (BUILD_CODE_COVERAGE)
    message("Enabling code-coverage options")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
endif()

##################
# Unit tests
##
if(BUILD_TESTING)
  message("Enabling unit-testing")
  enable_testing()
  add_subdirectory(test)
endif()


if (BUILD_PYTHON)
    message("Enabling python module")
    add_custom_target(python-module
        ALL
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/python
        COMMAND bash ${PROJECT_SOURCE_DIR}/python/build.sh ${PROJECT_BINARY_DIR}
    )
endif()

###############
# Installation
##

include(GNUInstallDirs)

set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/NodeJS)

install(TARGETS NodeJS
  EXPORT NodeJSTargets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT NodeJSTargets
  FILE NodeJSTargets.cmake
  NAMESPACE NodeJS::
  DESTINATION ${INSTALL_CONFIGDIR}
)

