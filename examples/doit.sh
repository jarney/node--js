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

mkdir -p xsd
for schema_object in ${SCHEMA_OBJECTS} ; do
    xsltproc schema-to-clean.xsl schema/${schema_object}.nodejs-schema >xsd/${schema_object}.xsd
done

../build/examples/validator xsd/org.ensor.nodejs.xsd ../test-data/xml/Serializer/org.ensor.nodejs.core.xml
../build/examples/validator xsd/org.ensor.nodejs.xsd ../test-data/xml/Serializer/org.ensor.nodejs.openscad.xml

mkdir -p include
mkdir -p src
for schema_object in ${SCHEMA_OBJECTS} ; do
    xsltproc schema-to-hpp.xsl schema/${schema_object}.nodejs-schema >include/${schema_object}.hpp
    xsltproc schema-to-cpp.xsl schema/${schema_object}.nodejs-schema >src/${schema_object}.cpp
done
