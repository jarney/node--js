#include <catch2/catch_all.hpp>
#include <iostream>
#include <fstream>

#include "node--js/xml/SerializerXML.hpp"
#include "node--js/xml/ModuleLoaderNodeJSPath.hpp"
#include "node--js/SerializerError.hpp"
#include "TestData.h"

using namespace NodeJS::core;
using namespace NodeJS::xml;
using namespace NodeJS::test;

static void testPackageFile(std::string packageName, std::function<void(bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter)> callback)
{
    std::ifstream xmlData(testFilename("../test-data/xml/Serializer/" + packageName + ".xml"));
  
    const auto & serializer = SerializerXML::instance();

    SerializerErrorReporterByCode errorReporter;
  
    ModuleLoader loader;
    auto nodeModule = loader.newModule("");
  
    bool result = serializer.read(packageName, xmlData, *nodeModule, errorReporter);
  
    callback(result, *nodeModule, errorReporter);
}

TEST_CASE("test_SerializerXML_openscad_package", "[NodeJS][xml][Serializer]") {
    std::string packageName = "org.ensor.nodejs.openscad";
    testPackageFile(packageName, [&packageName](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
      errorReporter.dump();
      CHECK(result);
      CHECK(!errorReporter.hasError());
      CHECK(!errorReporter.hasErrorCode(SerializerErrorCode::PACKAGE_ID_MISSING));
      CHECK(nodeModule.getPackage() == packageName);
    });
}

TEST_CASE("test_SerializerXML_valid_graph", "[NodeJS][xml][Serializer]") {
    std::string packageName = "valid-graph";
    testPackageFile(packageName, [](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
      errorReporter.dump();
      CHECK(result);
      CHECK(!errorReporter.hasError());
    });
}
TEST_CASE("test_SerializerXML_valid_minimal", "[NodeJS][xml][Serializer]") {
    std::string packageName = "valid-package-minimal";
    testPackageFile(packageName, [&packageName](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
      CHECK(result);
      CHECK(!errorReporter.hasError());
      CHECK(nodeModule.getPackage() == packageName);
      });
}
TEST_CASE("test_SerializerXML_package_version_missing", "[NodeJS][xml][Serializer]")
{
    std::string packageName("invalid-package-version-missing");
    testPackageFile(packageName, [](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
	    CHECK(!result);
	    CHECK(errorReporter.hasErrorCode(SerializerErrorCode::PACKAGE_VERSION_MISSING));
    });
}
TEST_CASE("test_SerializerXML_package_version_unsupported", "[NodeJS][xml][Serializer]") {
    std::string packageName = "invalid-package-version-unsupported";
    testPackageFile(packageName, [](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
      CHECK(!result);
      CHECK(errorReporter.hasErrorCode(SerializerErrorCode::PACKAGE_VERSION_UNSUPPORTED));
    });
}
TEST_CASE("test_SerializerXML_package_missing", "[NodeJS][xml][Serializer]") {
    std::string packageName = "invalid-package-missing";
    testPackageFile(packageName, [](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
      CHECK(!result);
      CHECK(errorReporter.hasError());
      CHECK(errorReporter.hasErrorCode(SerializerErrorCode::PACKAGE_MISSING));
      CHECK(nodeModule.getPackage() == "");
    });
}
TEST_CASE("test_SerializerXML_package_missing_id", "[NodeJS][xml][Serializer]") {
    std::string packageName = "invalid-package-missing-id";
    testPackageFile(packageName, [](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
      errorReporter.dump();
      CHECK(!result);
      CHECK(errorReporter.hasError());
      CHECK(errorReporter.hasErrorCode(SerializerErrorCode::PACKAGE_ID_MISSING));
      CHECK(nodeModule.getPackage() == "");
    });
}
TEST_CASE("test_SerializerXML_package_inconsistent", "[NodeJS][xml][Serializer]") {
    std::string packageName = "invalid-package-inconsistent";
    testPackageFile(packageName, [](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
      CHECK(!result);
      CHECK(errorReporter.hasError());
      CHECK(errorReporter.hasErrorCode(SerializerErrorCode::PACKAGE_ID_INCONSISTENT));
      CHECK(nodeModule.getPackage() == "");
    });
}
TEST_CASE("test_SerializerXML_package_duplicate", "[NodeJS][xml][Serializer]") {
    std::string packageName = "invalid-package-duplicate";
    testPackageFile(packageName, [](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
      CHECK(!result);
      CHECK(errorReporter.hasError());
      CHECK(errorReporter.hasErrorCode(SerializerErrorCode::PACKAGE_DUPLICATE));
    });
}
TEST_CASE("test_SerializerXML_data_type_id_missing", "[NodeJS][xml][Serializer]") {
    std::string packageName = "invalid-data-type-id-missing";
    testPackageFile(packageName, [](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
      CHECK(!result);
      CHECK(errorReporter.hasError());
      CHECK(errorReporter.hasErrorCode(SerializerErrorCode::DATA_TYPE_ID_MISSING));
    });
}
TEST_CASE("test_SerializerXML_data_type_id_duplicate", "[NodeJS][xml][Serializer]") {
    std::string packageName = "invalid-data-type-id-duplicate";
    testPackageFile(packageName, [](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
      CHECK(!result);
      CHECK(errorReporter.hasError());
      CHECK(errorReporter.hasErrorCode(SerializerErrorCode::DATA_TYPE_ID_DUPLICATE));
    });
}

TEST_CASE("test_SerializerXML_node_type_id_missing", "[NodeJS][xml][Serializer]") {
    std::string packageName = "invalid-node-type-id-missing";
    testPackageFile(packageName, [](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
      CHECK(!result);
      CHECK(errorReporter.hasError());
      CHECK(errorReporter.hasErrorCode(SerializerErrorCode::NODE_TYPE_ID_MISSING));
    });
}
TEST_CASE("test_SerializerXML_node_type_id_duplicate", "[NodeJS][xml][Serializer]") {
    std::string packageName = "invalid-node-type-id-duplicate";
    testPackageFile(packageName, [](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
      CHECK(!result);
      CHECK(errorReporter.hasError());
      CHECK(errorReporter.hasErrorCode(SerializerErrorCode::NODE_TYPE_ID_DUPLICATE));
    });
}
TEST_CASE("test_SerializerXML_node_type_visibility_missing", "[NodeJS][xml][Serializer]") {
    std::string packageName = "invalid-node-type-visibility-missing";
    testPackageFile(packageName, [](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
      CHECK(!result);
      CHECK(errorReporter.hasError());
      CHECK(errorReporter.hasErrorCode(SerializerErrorCode::NODE_TYPE_VISIBILITY_MISSING));
    });
}
TEST_CASE("test_SerializerXML_node_type_visibility_invalid", "[NodeJS][xml][Serializer]") {
    std::string packageName = "invalid-node-type-visibility-invalid";
    testPackageFile(packageName, [](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
      CHECK(!result);
      CHECK(errorReporter.hasError());
      CHECK(errorReporter.hasErrorCode(SerializerErrorCode::NODE_TYPE_VISIBILITY_INVALID));
    });
}
TEST_CASE("test_SerializerXML_node_type_type_missing", "[NodeJS][xml][Serializer]") {
    std::string packageName = "invalid-node-type-type-missing";
    testPackageFile(packageName, [](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
      CHECK(!result);
      CHECK(errorReporter.hasError());
      CHECK(errorReporter.hasErrorCode(SerializerErrorCode::NODE_TYPE_TYPE_MISSING));
    });
}
TEST_CASE("test_SerializerXML_node_type_type_invalid", "[NodeJS][xml][Serializer]") {
    std::string packageName = "invalid-node-type-type-invalid";
    testPackageFile(packageName, [](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
      CHECK(!result);
      CHECK(errorReporter.hasError());
      CHECK(errorReporter.hasErrorCode(SerializerErrorCode::NODE_TYPE_TYPE_INVALID));
    });
}
TEST_CASE("test_SerializerXML_metadata_namespace_missing", "[NodeJS][xml][Serializer]") {
    std::string packageName = "invalid-metadata-namespace-missing";
    testPackageFile(packageName, [](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
      CHECK(!result);
      CHECK(errorReporter.hasError());
      CHECK(errorReporter.hasErrorCode(SerializerErrorCode::METADATA_NAMESPACE_MISSING));
    });
}
TEST_CASE("test_SerializerXML_metadata_namespace_duplicate", "[NodeJS][xml][Serializer]") {
    std::string packageName = "invalid-metadata-namespace-duplicate";
    testPackageFile(packageName, [](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
      CHECK(!result);
      CHECK(errorReporter.hasError());
      CHECK(errorReporter.hasErrorCode(SerializerErrorCode::METADATA_NAMESPACE_DUPLICATE));
    });
}
TEST_CASE("test_SerializerXML_metadata_key_missing", "[NodeJS][xml][Serializer]") {
    std::string packageName = "invalid-metadata-key-missing";
    testPackageFile(packageName, [](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
      CHECK(!result);
      CHECK(errorReporter.hasError());
      CHECK(errorReporter.hasErrorCode(SerializerErrorCode::METADATA_KEY_MISSING));
    });
}
TEST_CASE("test_SerializerXML_metadata_key_duplicate", "[NodeJS][xml][Serializer]") {
    std::string packageName = "invalid-metadata-key-duplicate";
    testPackageFile(packageName, [](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
      CHECK(!result);
      CHECK(errorReporter.hasError());
      CHECK(errorReporter.hasErrorCode(SerializerErrorCode::METADATA_KEY_DUPLICATE));
    });
}
TEST_CASE("test_SerializerXML_port_id_missing", "[NodeJS][xml][Serializer]") {
    std::string packageName = "invalid-port-id-missing";
    testPackageFile(packageName, [](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
      CHECK(!result);
      CHECK(errorReporter.hasError());
      CHECK(errorReporter.hasErrorCode(SerializerErrorCode::PORT_ID_MISSING));
    });
}
TEST_CASE("test_SerializerXML_port_id_duplicate", "[NodeJS][xml][Serializer]") {
    std::string packageName = "invalid-port-id-duplicate";
    testPackageFile(packageName, [](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
      CHECK(!result);
      CHECK(errorReporter.hasError());
      CHECK(errorReporter.hasErrorCode(SerializerErrorCode::PORT_ID_DUPLICATE));
    });
}
TEST_CASE("test_SerializerXML_port_data_type_missing", "[NodeJS][xml][Serializer]") {
    std::string packageName = "invalid-port-data-type-missing";
    testPackageFile(packageName, [](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
      CHECK(!result);
      CHECK(errorReporter.hasError());
      CHECK(errorReporter.hasErrorCode(SerializerErrorCode::PORT_DATA_TYPE_MISSING));
    });
}
TEST_CASE("test_SerializerXML_port_data_type_invalid", "[NodeJS][xml][Serializer]") {
    std::string packageName = "invalid-port-data-type-invalid";
    testPackageFile(packageName, [](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
      CHECK(!result);
      CHECK(errorReporter.hasError());
      CHECK(errorReporter.hasErrorCode(SerializerErrorCode::PORT_DATA_TYPE_INVALID));
    });
}
  // THIS CONDITION IS DELIBERATELY NOT ENFORCED.  Connection policy should be optional and default to one.
  //it("xxx_SerializerXML_port_connection_policy_missing", "[NodeJS][xml][Serializer]") {
  //  std::string packageName = "invalid-port-connection-policy-missing";
  //  testPackageFile(packageName, [](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
  //    errorReporter.dump();
  //    CHECK(!result);
  //    CHECK(errorReporter.hasError());
  //    CHECK(errorReporter.hasErrorCode(SerializerErrorCode::PORT_CONNECTION_POLICY_MISSING));
  //  }
  //});
TEST_CASE("test_SerializerXML_port_connection_policy_invalid", "[NodeJS][xml][Serializer]") {
    std::string packageName = "invalid-port-connection-policy-invalid";
    testPackageFile(packageName, [](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
      CHECK(!result);
      CHECK(errorReporter.hasError());
      CHECK(errorReporter.hasErrorCode(SerializerErrorCode::PORT_CONNECTION_POLICY_INVALID));
    });
}

TEST_CASE("test_SerializerXML_graph_id_missing", "[NodeJS][xml][Serializer]") {
    std::string packageName = "invalid-graph-id-missing";
    testPackageFile(packageName, [](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
      CHECK(!result);
      CHECK(errorReporter.hasError());
      CHECK(errorReporter.hasErrorCode(SerializerErrorCode::GRAPH_ID_MISSING));
    });
}
TEST_CASE("test_SerializerXML_graph_id_duplicate", "[NodeJS][xml][Serializer]") {
    std::string packageName = "invalid-graph-id-duplicate";
    testPackageFile(packageName, [](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
      CHECK(!result);
      CHECK(errorReporter.hasError());
      CHECK(errorReporter.hasErrorCode(SerializerErrorCode::GRAPH_ID_DUPLICATE));
    });
}
TEST_CASE("test_SerializerXML_graph_cycle", "[NodeJS][xml][Serializer]") {
    std::string packageName = "invalid-graph-cycle";
    testPackageFile(packageName, [](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
      CHECK(!result);
      CHECK(errorReporter.hasError());
      CHECK(errorReporter.hasErrorCode(SerializerErrorCode::GRAPH_CYCLE));
    });
}
TEST_CASE("test_SerializerXML_node_id_missing", "[NodeJS][xml][Serializer]") {
    std::string packageName = "invalid-node-id-missing";
    testPackageFile(packageName, [](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
      CHECK(!result);
      CHECK(errorReporter.hasError());
      CHECK(errorReporter.hasErrorCode(SerializerErrorCode::NODE_NODE_ID_MISSING));
    });
}
TEST_CASE("test_SerializerXML_node_id_duplicate", "[NodeJS][xml][Serializer]") {
    std::string packageName = "invalid-node-id-duplicate";
    testPackageFile(packageName, [](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
      CHECK(!result);
      CHECK(errorReporter.hasError());
      CHECK(errorReporter.hasErrorCode(SerializerErrorCode::NODE_NODE_ID_DUPLICATE));
    });
}
TEST_CASE("test_SerializerXML_node_node_type_missing", "[NodeJS][xml][Serializer]") {
    std::string packageName = "invalid-node-node-type-missing";
    testPackageFile(packageName, [](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
      CHECK(!result);
      CHECK(errorReporter.hasError());
      CHECK(errorReporter.hasErrorCode(SerializerErrorCode::NODE_NODE_TYPE_MISSING));
    });
}
TEST_CASE("test_SerializerXML_node_node_type_invalid", "[NodeJS][xml][Serializer]") {
    std::string packageName = "invalid-node-node-type-invalid";
    testPackageFile(packageName, [](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
      CHECK(!result);
      CHECK(errorReporter.hasError());
      CHECK(errorReporter.hasErrorCode(SerializerErrorCode::NODE_NODE_TYPE_INVALID));
    });
}
TEST_CASE("test_SerializerXML_node_data_key_missing", "[NodeJS][xml][Serializer]") {
    std::string packageName = "invalid-node-data-key-missing";
    testPackageFile(packageName, [](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
      CHECK(!result);
      CHECK(errorReporter.hasError());
      CHECK(errorReporter.hasErrorCode(SerializerErrorCode::NODE_DATA_KEY_MISSING));
    });
}
TEST_CASE("test_SerializerXML_edge_duplicate", "[NodeJS][xml][Serializer]") {
    std::string packageName = "invalid-edge-duplicate";
    testPackageFile(packageName, [](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
      CHECK(!result);
      CHECK(errorReporter.hasError());
      CHECK(errorReporter.hasErrorCode(SerializerErrorCode::EDGE_DUPLICATE));
    });
}
TEST_CASE("test_SerializerXML_edge_data_mismatch", "[NodeJS][xml][Serializer]") {
    std::string packageName = "invalid-edge-data-mismatch";
    testPackageFile(packageName, [](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
      CHECK(!result);
      CHECK(errorReporter.hasError());
      CHECK(errorReporter.hasErrorCode(SerializerErrorCode::EDGE_PORT_DATA_MISMATCH));
    });
}
TEST_CASE("test_SerializerXML_edge_from_node_missing", "[NodeJS][xml][Serializer]") {
    std::string packageName = "invalid-edge-from-node-missing";
    testPackageFile(packageName, [](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
      CHECK(!result);
      CHECK(errorReporter.hasError());
      CHECK(errorReporter.hasErrorCode(SerializerErrorCode::EDGE_FROM_NODE_MISSING));
    });
}
TEST_CASE("test_SerializerXML_edge_from_port_missing", "[NodeJS][xml][Serializer]") {
    std::string packageName = "invalid-edge-from-port-missing";
    testPackageFile(packageName, [](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
      CHECK(!result);
      CHECK(errorReporter.hasError());
      CHECK(errorReporter.hasErrorCode(SerializerErrorCode::EDGE_FROM_PORT_MISSING));
    });
}
TEST_CASE("test_SerializerXML_edge_to_node_missing", "[NodeJS][xml][Serializer]") {
    std::string packageName = "invalid-edge-to-node-missing";
    testPackageFile(packageName, [](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
      CHECK(!result);
      CHECK(errorReporter.hasError());
      CHECK(errorReporter.hasErrorCode(SerializerErrorCode::EDGE_TO_NODE_MISSING));
    });
}
TEST_CASE("test_SerializerXML_edge_to_port_missing", "[NodeJS][xml][Serializer]") {
    std::string packageName = "invalid-edge-to-port-missing";
    testPackageFile(packageName, [](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
      CHECK(!result);
      CHECK(errorReporter.hasError());
      CHECK(errorReporter.hasErrorCode(SerializerErrorCode::EDGE_TO_PORT_MISSING));
    });
}
TEST_CASE("test_SerializerXML_edge_from_node_invalid", "[NodeJS][xml][Serializer]") {
    std::string packageName = "invalid-edge-from-node-invalid";
    testPackageFile(packageName, [](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
      CHECK(!result);
      CHECK(errorReporter.hasError());
      CHECK(errorReporter.hasErrorCode(SerializerErrorCode::EDGE_FROM_NODE_INVALID));
    });
}
TEST_CASE("test_SerializerXML_edge_from_port_invalid", "[NodeJS][xml][Serializer]") {
    std::string packageName = "invalid-edge-from-port-invalid";
    testPackageFile(packageName, [](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
      CHECK(!result);
      CHECK(errorReporter.hasError());
      CHECK(errorReporter.hasErrorCode(SerializerErrorCode::EDGE_FROM_PORT_INVALID));
    });
}
TEST_CASE("test_SerializerXML_edge_to_node_invalid", "[NodeJS][xml][Serializer]") {
    std::string packageName = "invalid-edge-to-node-invalid";
    testPackageFile(packageName, [](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
      CHECK(!result);
      CHECK(errorReporter.hasError());
      CHECK(errorReporter.hasErrorCode(SerializerErrorCode::EDGE_TO_NODE_INVALID));
    });
}
TEST_CASE("test_SerializerXML_edge_to_port_invalid", "[NodeJS][xml][Serializer]") {
    std::string packageName = "invalid-edge-to-port-invalid";
    testPackageFile(packageName, [](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
      CHECK(!result);
      CHECK(errorReporter.hasError());
      CHECK(errorReporter.hasErrorCode(SerializerErrorCode::EDGE_TO_PORT_INVALID));
    });
}
TEST_CASE("test_SerializerXML_edge_connection_policy_from", "[NodeJS][xml][Serializer]") {
    std::string packageName = "invalid-edge-connection-policy-from";
    testPackageFile(packageName, [](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
      CHECK(!result);
      CHECK(errorReporter.hasError());
      CHECK(errorReporter.hasErrorCode(SerializerErrorCode::EDGE_VIOLATES_POLICY));
    });
}
TEST_CASE("test_SerializerXML_edge_connection_policy_to", "[NodeJS][xml][Serializer]") {
    std::string packageName = "invalid-edge-connection-policy-to";
    testPackageFile(packageName, [](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
      CHECK(!result);
      CHECK(errorReporter.hasError());
      CHECK(errorReporter.hasErrorCode(SerializerErrorCode::EDGE_VIOLATES_POLICY));
    });
}

