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
  it("test_SerializerXML_valid_minimal", function() {
    var packageName: string = "valid-package-minimal";
    testPackageFile(packageName, function(result: boolean, nodeModule: NodeModule, errorReporter: SerializerErrorReporterByCode) {
      expect.assert(result);
      expect.assert(!errorReporter.hasError());
      expect.assert(nodeModule.getPackage() == packageName);
      });
  });
});
