#########################################
# Validator utility to validate xml files
# and make sure they comply with the
# official schema.
#########################################
set(VALIDATOR_SOURCE_FILES
    val.cpp
)
set(VALIDATOR_HEADER_FILES

)

add_executable(validator
  ${VALIDATOR_SOURCE_FILES}
  ${VALIDATOR_HEADER_FILES}
)

target_link_libraries(validator NodeJS)

#########################################
# This is a simple 'loopback'
# kind-of test which reads a module,
# parses it for validity and writes
# it back out again.
#########################################
set(PASSTHROUGH_SOURCE_FILES
  passthrough.cpp
)

set(PASSTHROUGH_HEADER_FILES

)

add_executable(passthrough
  ${PASSTHROUGH_SOURCE_FILES}
  ${PASSTHROUGH_HEADER_FILES}
)

target_link_libraries(passthrough NodeJS)

#########################################
# This is just a core 'process' utility
# which reads a graph, parses it for
# validity, and then executes the 'main'
# graph on it.  There are a number of
# practical examples that can be done with
# this alone such as generating makfiles,
# openscad, and even lisp code.
#########################################

set(PROCESS_SOURCE_FILES
  process.cpp
)

set(PROCESS_HEADER_FILES

)

add_executable(process
  ${PROCESS_SOURCE_FILES}
)
target_include_directories(process PUBLIC ${Boost_INCLUDE_DIR} )

target_link_libraries(process NodeJS ${Boost_LIBRARIES})

#########################################
# Generated header.
#########################################
add_executable(gen)

set(GENERATED_INCLUDE_DIR ${PROJECT_BINARY_DIR}/generated/include)
set(GENERATED_SRC_DIR ${PROJECT_BINARY_DIR}/generated/src)
set(GENERATED_XSD_DIR ${PROJECT_BINARY_DIR}/generated/xsd)

add_custom_command(
    OUTPUT
        ${GENERATED_INCLUDE_DIR}/DataType.hpp
        ${GENERATED_INCLUDE_DIR}/DataTypeList.hpp
        ${GENERATED_INCLUDE_DIR}/Edge.hpp
        ${GENERATED_INCLUDE_DIR}/EdgeList.hpp
        ${GENERATED_INCLUDE_DIR}/Graph.hpp
        ${GENERATED_INCLUDE_DIR}/GraphArgument.hpp
        ${GENERATED_INCLUDE_DIR}/GraphArgumentList.hpp
        ${GENERATED_INCLUDE_DIR}/GraphArgumentType.hpp
        ${GENERATED_INCLUDE_DIR}/GraphList.hpp
        ${GENERATED_INCLUDE_DIR}/GraphResult.hpp
        ${GENERATED_INCLUDE_DIR}/GraphResultList.hpp
        ${GENERATED_INCLUDE_DIR}/Import.hpp
        ${GENERATED_INCLUDE_DIR}/Metadata.hpp
        ${GENERATED_INCLUDE_DIR}/MetadataWithNamespace.hpp
        ${GENERATED_INCLUDE_DIR}/MetadataWithoutNamespace.hpp
        ${GENERATED_INCLUDE_DIR}/NameValuePair.hpp
        ${GENERATED_INCLUDE_DIR}/Node.hpp
        ${GENERATED_INCLUDE_DIR}/NodeList.hpp
        ${GENERATED_INCLUDE_DIR}/NodeModule.hpp
        ${GENERATED_INCLUDE_DIR}/NodeType.hpp
        ${GENERATED_INCLUDE_DIR}/NodeTypeAllowPortOverride.hpp
        ${GENERATED_INCLUDE_DIR}/NodeTypeList.hpp
        ${GENERATED_INCLUDE_DIR}/NodeTypeType.hpp
        ${GENERATED_INCLUDE_DIR}/NodeTypeVisibility.hpp
        ${GENERATED_INCLUDE_DIR}/Package.hpp
        ${GENERATED_INCLUDE_DIR}/Port.hpp
        ${GENERATED_INCLUDE_DIR}/PortConnectionPolicy.hpp
        ${GENERATED_INCLUDE_DIR}/PortList.hpp
        ${GENERATED_INCLUDE_DIR}/Scope.hpp

        ${GENERATED_SRC_DIR}/DataType.cpp
        ${GENERATED_SRC_DIR}/DataTypeList.cpp
        ${GENERATED_SRC_DIR}/Edge.cpp
        ${GENERATED_SRC_DIR}/EdgeList.cpp
        ${GENERATED_SRC_DIR}/Graph.cpp
        ${GENERATED_SRC_DIR}/GraphArgument.cpp
        ${GENERATED_SRC_DIR}/GraphArgumentList.cpp
        ${GENERATED_SRC_DIR}/GraphArgumentType.cpp
        ${GENERATED_SRC_DIR}/GraphList.cpp
        ${GENERATED_SRC_DIR}/GraphResult.cpp
        ${GENERATED_SRC_DIR}/GraphResultList.cpp
        ${GENERATED_SRC_DIR}/Import.cpp
        ${GENERATED_SRC_DIR}/Metadata.cpp
        ${GENERATED_SRC_DIR}/MetadataWithNamespace.cpp
        ${GENERATED_SRC_DIR}/MetadataWithoutNamespace.cpp
        ${GENERATED_SRC_DIR}/NameValuePair.cpp
        ${GENERATED_SRC_DIR}/Node.cpp
        ${GENERATED_SRC_DIR}/NodeList.cpp
        ${GENERATED_SRC_DIR}/NodeModule.cpp
        ${GENERATED_SRC_DIR}/NodeType.cpp
        ${GENERATED_SRC_DIR}/NodeTypeAllowPortOverride.cpp
        ${GENERATED_SRC_DIR}/NodeTypeList.cpp
        ${GENERATED_SRC_DIR}/NodeTypeType.cpp
        ${GENERATED_SRC_DIR}/NodeTypeVisibility.cpp
        ${GENERATED_SRC_DIR}/Package.cpp
        ${GENERATED_SRC_DIR}/Port.cpp
        ${GENERATED_SRC_DIR}/PortConnectionPolicy.cpp
        ${GENERATED_SRC_DIR}/PortList.cpp
        ${GENERATED_SRC_DIR}/Scope.cpp
    COMMAND
        sh ${PROJECT_SOURCE_DIR}/examples/generate-schema.sh
	    ${PROJECT_SOURCE_DIR}/schema
	    ${PROJECT_SOURCE_DIR}/examples
	    ${GENERATED_XSD_DIR}
	    ${GENERATED_INCLUDE_DIR}
	    ${GENERATED_SRC_DIR}
    DEPENDS
        ${PROJECT_SOURCE_DIR}/schema/DataType.nodejs-schema
        ${PROJECT_SOURCE_DIR}/schema/DataTypeList.nodejs-schema
        ${PROJECT_SOURCE_DIR}/schema/Edge.nodejs-schema
        ${PROJECT_SOURCE_DIR}/schema/EdgeList.nodejs-schema
        ${PROJECT_SOURCE_DIR}/schema/Graph.nodejs-schema
        ${PROJECT_SOURCE_DIR}/schema/GraphArgument.nodejs-schema
        ${PROJECT_SOURCE_DIR}/schema/GraphArgumentList.nodejs-schema
        ${PROJECT_SOURCE_DIR}/schema/GraphArgumentType.nodejs-schema
        ${PROJECT_SOURCE_DIR}/schema/GraphList.nodejs-schema
        ${PROJECT_SOURCE_DIR}/schema/GraphResult.nodejs-schema
        ${PROJECT_SOURCE_DIR}/schema/GraphResultList.nodejs-schema
        ${PROJECT_SOURCE_DIR}/schema/Import.nodejs-schema
        ${PROJECT_SOURCE_DIR}/schema/Metadata.nodejs-schema
        ${PROJECT_SOURCE_DIR}/schema/MetadataWithNamespace.nodejs-schema
        ${PROJECT_SOURCE_DIR}/schema/MetadataWithoutNamespace.nodejs-schema
        ${PROJECT_SOURCE_DIR}/schema/NameValuePair.nodejs-schema
        ${PROJECT_SOURCE_DIR}/schema/Node.nodejs-schema
        ${PROJECT_SOURCE_DIR}/schema/NodeList.nodejs-schema
        ${PROJECT_SOURCE_DIR}/schema/NodeModule.nodejs-schema
        ${PROJECT_SOURCE_DIR}/schema/NodeType.nodejs-schema
        ${PROJECT_SOURCE_DIR}/schema/NodeTypeAllowPortOverride.nodejs-schema
        ${PROJECT_SOURCE_DIR}/schema/NodeTypeList.nodejs-schema
        ${PROJECT_SOURCE_DIR}/schema/NodeTypeType.nodejs-schema
        ${PROJECT_SOURCE_DIR}/schema/NodeTypeVisibility.nodejs-schema
        ${PROJECT_SOURCE_DIR}/schema/Package.nodejs-schema
        ${PROJECT_SOURCE_DIR}/schema/Port.nodejs-schema
        ${PROJECT_SOURCE_DIR}/schema/PortConnectionPolicy.nodejs-schema
        ${PROJECT_SOURCE_DIR}/schema/PortList.nodejs-schema
        ${PROJECT_SOURCE_DIR}/schema/Scope.nodejs-schema
        ${CMAKE_CURRENT_SOURCE_DIR}/schema-to-hpp.xsl
        ${CMAKE_CURRENT_SOURCE_DIR}/schema-to-cpp.xsl
        ${CMAKE_CURRENT_SOURCE_DIR}/schema-to-xsd.xsl
  VERBATIM
    )

target_include_directories(gen PUBLIC ${GENERATED_INCLUDE_DIR})
target_sources(gen PRIVATE
    gen.cpp 
    ${GENERATED_SRC_DIR}/DataType.cpp
    ${GENERATED_SRC_DIR}/DataTypeList.cpp
    ${GENERATED_SRC_DIR}/Edge.cpp
    ${GENERATED_SRC_DIR}/EdgeList.cpp
    ${GENERATED_SRC_DIR}/Graph.cpp
    ${GENERATED_SRC_DIR}/GraphArgument.cpp
    ${GENERATED_SRC_DIR}/GraphArgumentList.cpp
    ${GENERATED_SRC_DIR}/GraphArgumentType.cpp
    ${GENERATED_SRC_DIR}/GraphList.cpp
    ${GENERATED_SRC_DIR}/GraphResult.cpp
    ${GENERATED_SRC_DIR}/GraphResultList.cpp
    ${GENERATED_SRC_DIR}/Import.cpp
    ${GENERATED_SRC_DIR}/Metadata.cpp
    ${GENERATED_SRC_DIR}/MetadataWithNamespace.cpp
    ${GENERATED_SRC_DIR}/MetadataWithoutNamespace.cpp
    ${GENERATED_SRC_DIR}/NameValuePair.cpp
    ${GENERATED_SRC_DIR}/Node.cpp
    ${GENERATED_SRC_DIR}/NodeList.cpp
    ${GENERATED_SRC_DIR}/NodeModule.cpp
    ${GENERATED_SRC_DIR}/NodeType.cpp
    ${GENERATED_SRC_DIR}/NodeTypeAllowPortOverride.cpp
    ${GENERATED_SRC_DIR}/NodeTypeList.cpp
    ${GENERATED_SRC_DIR}/NodeTypeType.cpp
    ${GENERATED_SRC_DIR}/NodeTypeVisibility.cpp
    ${GENERATED_SRC_DIR}/Package.cpp
    ${GENERATED_SRC_DIR}/Port.cpp
    ${GENERATED_SRC_DIR}/PortConnectionPolicy.cpp
    ${GENERATED_SRC_DIR}/PortList.cpp
    ${GENERATED_SRC_DIR}/Scope.cpp
    )
