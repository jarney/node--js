#!/bin/sh

if [ $# -ne 5 ] ; then
    echo "Usage: generate-schema.sh schema-directory xsd-dir c-include-dir c-source-dir"
    exit 1
fi
set -e
    
SCHEMA_DIRECTORY=$1
STYLESHEET_DIRECTORY=$2
XSD_DIRECTORY=$3
C_INCLUDE_DIRECTORY=$4
C_SOURCE_DIRECTORY=$5

SCHEMA_OBJECTS="
    DataType
    DataTypeList
    Edge
    EdgeList
    Graph
    GraphArgument
    GraphArgumentList
    GraphArgumentType
    GraphList
    GraphResult
    GraphResultList
    Import
    Metadata
    MetadataWithNamespace
    MetadataWithoutNamespace
    NameValuePair
    Node
    NodeList
    NodeModule
    NodeType
    NodeTypeAllowPortOverride
    NodeTypeList
    NodeTypeType
    NodeTypeVisibility
    Package
    Port
    PortConnectionPolicy
    PortList
    Scope
"

# Generate clean XSD for validation purposes.
mkdir -p ${XSD_DIRECTORY}
for schema_object in ${SCHEMA_OBJECTS} ; do
    xsltproc ${STYLESHEET_DIRECTORY}/schema-to-xsd.xsl \
	     ${SCHEMA_DIRECTORY}/${schema_object}.nodejs-schema \
	     >${XSD_DIRECTORY}/${schema_object}.xsd
done

# Generate C++ header and class files.
mkdir -p ${C_INCLUDE_DIRECTORY}
mkdir -p ${C_SOURCE_DIRECTORY}
for schema_object in ${SCHEMA_OBJECTS} ; do
    echo ${C_INCLUDE_DIRECTORY}/${schema_object}.hpp
    
    xsltproc ${STYLESHEET_DIRECTORY}/schema-to-hpp.xsl \
	     ${SCHEMA_DIRECTORY}/${schema_object}.nodejs-schema \
	     >${C_INCLUDE_DIRECTORY}/${schema_object}.hpp
    
    xsltproc ${STYLESHEET_DIRECTORY}/schema-to-cpp.xsl \
	     ${SCHEMA_DIRECTORY}/${schema_object}.nodejs-schema \
	     >${C_SOURCE_DIRECTORY}/${schema_object}.cpp
done

# TODO: Use schema to generate Python files.

# TODO: Use schema to generate Javascript/Typescript files for browser-version.

# Use validator to validate that the schema works to validate documents.
#../build/examples/validator xsd/org.ensor.nodejs.xsd ../test-data/xml/Serializer/org.ensor.nodejs.core.xml
#../build/examples/validator xsd/org.ensor.nodejs.xsd ../test-data/xml/Serializer/org.ensor.nodejs.openscad.xml

