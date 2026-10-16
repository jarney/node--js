import fs from "fs";

import { describe, it, expect } from "vitest";

import { NodeModule } from "../src/NodeModule";

import { SerializerXML } from "../src/xml/SerializerXML";
import { SerializerError, SerializerErrorCode, SerializerErrorReporterByCode } from "../src/SerializerError";

async function testPackageFile(
  packageName: string,
  callback: (result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) => void
) {
  var xmlData: string = fs.readFileSync("../test-data/xml/Serializer/" + packageName + ".xml", {encoding: "utf8", flag: "r"});
  
  var serializer: SerializerXML = SerializerXML.instance();
  var errorReporter: SerializerErrorReporter = new SerializerErrorReporterByCode();
  
  var nodeModule: NodeModule = new NodeModule();
  
  var result: boolean = await serializer.read(packageName, xmlData, nodeModule, errorReporter);
  
  callback(result, nodeModule, errorReporter);
}


describe("SerializerXML", function(){
  it("test_SerializerXML_openscad_package", function(){
    var packageName: string = "org.ensor.nodejs.openscad";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      expect.assert(result);
      expect.assert(!errorReporter.hasError());
      expect.assert(!errorReporter.hasErrorCode(SerializerErrorCode.PACKAGE_ID_MISSING));
      expect.assert(nodeModule.getPackage() == packageName);
    });
  });
  it("test_SerializerXML_valid_graph", function(){
    var packageName: string = "valid-graph";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      errorReporter.dump();
      expect.assert(result);
      expect.assert(!errorReporter.hasError());
    });
  });
  it("test_SerializerXML_valid_minimal", function() {
    var packageName: string = "valid-package-minimal";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      expect.assert(result);
      expect.assert(!errorReporter.hasError());
      expect.assert(nodeModule.getPackage() == packageName);
      });
  });
  it("test_SerializerXML_package_version_missing", function() {
    var packageName: string = "invalid-package-version-missing";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      expect.assert(!result);
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.PACKAGE_VERSION_MISSING));
      });
  });
  it("test_SerializerXML_package_version_unsupported", function() {
    var packageName: string = "invalid-package-version-unsupported";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      expect.assert(!result);
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.PACKAGE_VERSION_UNSUPPORTED));
      });
  });
  it("test_SerializerXML_package_missing", function() {
    var packageName: string = "invalid-package-missing";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.PACKAGE_MISSING));
      expect.assert(nodeModule.getPackage() == "");
      });
  });
  it("test_SerializerXML_package_missing_id", function() {
    var packageName: string = "invalid-package-missing-id";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.PACKAGE_ID_MISSING));
      expect.assert(nodeModule.getPackage() == "");
      });
  });
  it("test_SerializerXML_package_inconsistent", function() {
    var packageName: string = "invalid-package-inconsistent";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.PACKAGE_ID_INCONSISTENT));
      expect.assert(nodeModule.getPackage() == "");
      });
  });
  it("test_SerializerXML_package_duplicate", function() {
    var packageName: string = "invalid-package-duplicate";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.PACKAGE_DUPLICATE));
      });
  });
  it("test_SerializerXML_data_type_id_missing", function() {
    var packageName: string = "invalid-data-type-id-missing";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.DATA_TYPE_ID_MISSING));
      });
  });
  it("test_SerializerXML_data_type_id_duplicate", function() {
    var packageName: string = "invalid-data-type-id-duplicate";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.DATA_TYPE_ID_DUPLICATE));
      });
  });
  it("test_SerializerXML_node_type_id_missing", function() {
    var packageName: string = "invalid-node-type-id-missing";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.NODE_TYPE_ID_MISSING));
      });
  });
  it("test_SerializerXML_node_type_id_duplicate", function() {
    var packageName: string = "invalid-node-type-id-duplicate";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.NODE_TYPE_ID_DUPLICATE));
      });
  });
  it("test_SerializerXML_node_type_visibility_missing", function() {
    var packageName: string = "invalid-node-type-visibility-missing";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.NODE_TYPE_VISIBILITY_MISSING));
      });
  });
  it("test_SerializerXML_node_type_visibility_invalid", function() {
    var packageName: string = "invalid-node-type-visibility-invalid";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.NODE_TYPE_VISIBILITY_INVALID));
      });
  });
  it("test_SerializerXML_node_type_type_missing", function() {
    var packageName: string = "invalid-node-type-type-missing";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.NODE_TYPE_TYPE_MISSING));
      });
  });
  it("test_SerializerXML_node_type_type_invalid", function() {
    var packageName: string = "invalid-node-type-type-invalid";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.NODE_TYPE_TYPE_INVALID));
      });
  });
  it("test_SerializerXML_metadata_namespace_missing", function() {
    var packageName: string = "invalid-metadata-namespace-missing";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.METADATA_NAMESPACE_MISSING));
      });
  });
  it("test_SerializerXML_metadata_namespace_duplicate", function() {
    var packageName: string = "invalid-metadata-namespace-duplicate";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.METADATA_NAMESPACE_DUPLICATE));
      });
  });
  it("test_SerializerXML_metadata_key_missing", function() {
    var packageName: string = "invalid-metadata-key-missing";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.METADATA_KEY_MISSING));
      });
  });
  it("test_SerializerXML_metadata_key_duplicate", function() {
    var packageName: string = "invalid-metadata-key-duplicate";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.METADATA_KEY_DUPLICATE));
      });
  });
  it("test_SerializerXML_port_id_missing", function() {
    var packageName: string = "invalid-port-id-missing";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.PORT_ID_MISSING));
      });
  });
  it("test_SerializerXML_port_id_duplicate", function() {
    var packageName: string = "invalid-port-id-duplicate";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.PORT_ID_DUPLICATE));
      });
  });
  it("test_SerializerXML_port_data_type_missing", function() {
    var packageName: string = "invalid-port-data-type-missing";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.PORT_DATA_TYPE_MISSING));
      });
  });
  it("test_SerializerXML_port_data_type_invalid", function() {
    var packageName: string = "invalid-port-data-type-invalid";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.PORT_DATA_TYPE_INVALID));
      });
  });
  // THIS CONDITION IS DELIBERATELY NOT ENFORCED.  Connection policy should be optional and default to one.
  //it("xxx_SerializerXML_port_connection_policy_missing", function() {
  //  var packageName: string = "invalid-port-connection-policy-missing";
  //  testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
  //    errorReporter.dump();
  //    expect.assert(!result);
  //    expect.assert(errorReporter.hasError());
  //    expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.PORT_CONNECTION_POLICY_MISSING));
  //    });
  //});
  it("test_SerializerXML_port_connection_policy_invalid", function() {
    var packageName: string = "invalid-port-connection-policy-invalid";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.PORT_CONNECTION_POLICY_INVALID));
      });
  });
  it("test_SerializerXML_graph_id_missing", function() {
    var packageName: string = "invalid-graph-id-missing";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.GRAPH_ID_MISSING));
      });
  });
  it("test_SerializerXML_graph_id_duplicate", function() {
    var packageName: string = "invalid-graph-id-duplicate";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.GRAPH_ID_DUPLICATE));
      });
  });
  it("test_SerializerXML_graph_cycle", function() {
    var packageName: string = "invalid-graph-cycle";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      errorReporter.dump();
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.GRAPH_CYCLE));
    });
  });
  it("test_SerializerXML_node_id_missing", function() {
    var packageName: string = "invalid-node-id-missing";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.NODE_NODE_ID_MISSING));
      });
  });
  it("test_SerializerXML_node_id_duplicate", function() {
    var packageName: string = "invalid-node-id-duplicate";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.NODE_NODE_ID_DUPLICATE));
      });
  });
  it("test_SerializerXML_node_node_type_missing", function() {
    var packageName: string = "invalid-node-node-type-missing";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.NODE_NODE_TYPE_MISSING));
      });
  });
  it("test_SerializerXML_node_node_type_invalid", function() {
    var packageName: string = "invalid-node-node-type-invalid";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.NODE_NODE_TYPE_INVALID));
      });
  });
  it("test_SerializerXML_node_data_key_missing", function() {
    var packageName: string = "invalid-node-data-key-missing";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.NODE_DATA_KEY_MISSING));
      });
  });
  it("test_SerializerXML_edge_duplicate", function() {
    var packageName: string = "invalid-edge-duplicate";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.EDGE_DUPLICATE));
      });
  });
  it("test_SerializerXML_edge_data_mismatch", function() {
    var packageName: string = "invalid-edge-data-mismatch";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.EDGE_PORT_DATA_MISMATCH));
      });
  });
  it("test_SerializerXML_edge_from_node_missing", function() {
    var packageName: string = "invalid-edge-from-node-missing";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.EDGE_FROM_NODE_MISSING));
      });
  });
  it("test_SerializerXML_edge_from_port_missing", function() {
    var packageName: string = "invalid-edge-from-port-missing";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.EDGE_FROM_PORT_MISSING));
      });
  });
  it("test_SerializerXML_edge_to_node_missing", function() {
    var packageName: string = "invalid-edge-to-node-missing";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.EDGE_TO_NODE_MISSING));
      });
  });
  it("test_SerializerXML_edge_to_port_missing", function() {
    var packageName: string = "invalid-edge-to-port-missing";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.EDGE_TO_PORT_MISSING));
      });
  });
  it("test_SerializerXML_edge_from_node_invalid", function() {
    var packageName: string = "invalid-edge-from-node-invalid";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.EDGE_FROM_NODE_INVALID));
      });
  });
  it("test_SerializerXML_edge_from_port_invalid", function() {
    var packageName: string = "invalid-edge-from-port-invalid";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.EDGE_FROM_PORT_INVALID));
      });
  });
  it("test_SerializerXML_edge_to_node_invalid", function() {
    var packageName: string = "invalid-edge-to-node-invalid";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.EDGE_TO_NODE_INVALID));
    });
  });
  it("test_SerializerXML_edge_to_port_invalid", function() {
    var packageName: string = "invalid-edge-to-port-invalid";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.EDGE_TO_PORT_INVALID));
    });
  });
  it("test_SerializerXML_edge_connection_policy_from", function() {
    var packageName: string = "invalid-edge-connection-policy-from";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.EDGE_VIOLATES_POLICY));
    });
  });
  it("test_SerializerXML_edge_connection_policy_to", function() {
    var packageName: string = "invalid-edge-connection-policy-to";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.EDGE_VIOLATES_POLICY));
    });
  });
});
