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

TEST_CASE("test_xml_Serializer", "[NodeJS][xml][Serializer]")
{
    const auto & ser = SerializerXML::instance();

    std::string packageName("Test-Serializer-Basic");
    std::ifstream in(testFilename(packageName + ".xml"));

    SerializerErrorReporterStream err(std::cerr);
    
    ModuleLoaderNodeJSPath loader;
    NodeModule & nodeModule = *loader.newModule("anonymous");
    bool rc = ser.read(
	packageName,
	in,
	nodeModule,
	err);
    CHECK(rc);

    rc = ser.write(
	nodeModule,
	std::cout,
	err);
    CHECK(rc);

}

TEST_CASE("test_xml_Serializer xml parse error", "[NodeJS][xml][Serializer]")
{
    const auto & ser = SerializerXML::instance();

    std::string packageName("Test-xml-Serializer-parse-error");
    std::ifstream in(testFilename(packageName + ".xml"));

    SerializerErrorReporterByCode err;
    ModuleLoaderNodeJSPath loader;
    NodeModule & nodeModule = *loader.newModule("anonymous");
    bool rc = ser.read(
	packageName,
	in,
	nodeModule,
	err);
    CHECK(!rc);
    CHECK(err.size() == 1);
    CHECK(err.getErrors(SerializerErrorCode::LEGACY_ERROR_XML_PARSE).size() == 1);

}

TEST_CASE("test_xml_Serializer xml parse error with stream reporting", "[NodeJS][xml][Serializer]")
{
    const auto & ser = SerializerXML::instance();
    std::string packageName("Test-xml-Serializer-parse-error");
    std::ifstream in(testFilename(packageName + ".xml"));

    std::ostringstream ostring;
    SerializerErrorReporterStream err(ostring);
    ModuleLoaderNodeJSPath loader;
    NodeModule & nodeModule = *loader.newModule("anonymous");
    bool rc = ser.read(
	packageName,
	in,
	nodeModule,
	err);
    CHECK(!rc);

    std::string ostr = ostring.str();
    CHECK(ostr.find("Failed to parse xml document") != std::string::npos);
}

static void testPackageFile(std::string packageName, std::function<void(bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter)> callback)
{
    std::ifstream xmlData(testFilename("../test-data/xml/Serializer/" + packageName + ".xml"));
  
    const auto & serializer = SerializerXML::instance();

    SerializerErrorReporterByCode errorReporter;
  
    ModuleLoaderNodeJSPath loader;
    NodeModule & nodeModule = *loader.newModule("anonymous");
  
    bool result = serializer.read(packageName, xmlData, nodeModule, errorReporter);
  
    callback(result, nodeModule, errorReporter);
}

TEST_CASE("test_SerializerXML_package_version_missing", "[NodeJS][xml][Serializer]")
{
    std::string packageName("invalid-package-version-missing");
    testPackageFile(packageName, [](bool result, NodeModule & nodeModule, SerializerErrorReporterByCode & errorReporter) {
	    CHECK(!result);
	    CHECK(errorReporter.hasErrorCode(SerializerErrorCode::PACKAGE_VERSION_MISSING));
    });
}
