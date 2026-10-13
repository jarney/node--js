import fs from "fs";

import { describe, it, expect } from "vitest";

import { NodeModule } from "../src/NodeModule.ts";

import { SerializerXML } from "../src/xml/SerializerXML.ts";
import { SerializerError, SerializerErrorCode, SerializerErrorReporterByCode } from "../src/SerializerError.ts";

function testPackageFile(
  packageName: string,
  callback: (result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) => void
) {
  var xmlData: string = fs.readFileSync("../test-data/xml/Serializer/" + packageName + ".xml", {encoding: "utf8", flag: "r"});
  
  var serializer: SerializerXML = new SerializerXML();
  var errorReporter: SerializerErrorReporter = new SerializerErrorReporterByCode();
  
  var nodeModule: NodeModule = new NodeModule();
  
  var result: boolean = serializer.read(packageName, xmlData, nodeModule, errorReporter);
  
  callback(result, nodeModule, errorReporter);
}


describe("SerializerXML", function(){
  it("test_SerializerXML_openscad_package", function(){
    var packageName: string = "org.ensor.nodejs.openscad";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      errorReporter.dump()
      expect.assert(result);
      expect.assert(!errorReporter.hasError());
      expect.assert(!errorReporter.hasErrorCode(SerializerErrorCode.PACKAGE_ID_MISSING));
      expect.assert(nodeModule.getPackage() == packageName);
    });
  });
  it("test_SerializerXML_valid_graph", function(){
    var packageName: string = "valid-graph";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      errorReporter.dump()
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
      errorReporter.dump();
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.NODE_TYPE_ID_MISSING));
      });
  });
  it("test_SerializerXML_node_type_id_duplicate", function() {
    var packageName: string = "invalid-node-type-id-duplicate";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      errorReporter.dump();
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.NODE_TYPE_ID_DUPLICATE));
      });
  });
  it("test_SerializerXML_node_type_visibility_missing", function() {
    var packageName: string = "invalid-node-type-visibility-missing";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      errorReporter.dump();
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.NODE_TYPE_VISIBILITY_MISSING));
      });
  });
  it("test_SerializerXML_node_type_visibility_invalid", function() {
    var packageName: string = "invalid-node-type-visibility-invalid";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      errorReporter.dump();
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.NODE_TYPE_VISIBILITY_INVALID));
      });
  });
  it("test_SerializerXML_node_type_type_missing", function() {
    var packageName: string = "invalid-node-type-type-missing";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      errorReporter.dump();
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.NODE_TYPE_TYPE_MISSING));
      });
  });
  it("test_SerializerXML_node_type_type_invalid", function() {
    var packageName: string = "invalid-node-type-type-invalid";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      errorReporter.dump();
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.NODE_TYPE_TYPE_INVALID));
      });
  });
  it("test_SerializerXML_metadata_namespace_missing", function() {
    var packageName: string = "invalid-metadata-namespace-missing";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      errorReporter.dump();
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.METADATA_NAMESPACE_MISSING));
      });
  });
  it("test_SerializerXML_metadata_namespace_duplicate", function() {
    var packageName: string = "invalid-metadata-namespace-duplicate";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      errorReporter.dump();
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.METADATA_NAMESPACE_DUPLICATE));
      });
  });
  it("test_SerializerXML_metadata_key_missing", function() {
    var packageName: string = "invalid-metadata-key-missing";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      errorReporter.dump();
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.METADATA_KEY_MISSING));
      });
  });
  it("test_SerializerXML_metadata_key_duplicate", function() {
    var packageName: string = "invalid-metadata-key-duplicate";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      errorReporter.dump();
      expect.assert(!result);
      expect.assert(errorReporter.hasError());
      expect.assert(errorReporter.hasErrorCode(SerializerErrorCode.METADATA_KEY_DUPLICATE));
      });
  });
});
