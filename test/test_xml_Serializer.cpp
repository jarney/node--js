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

    std::ifstream in(testFilename("Test-Serializer-Basic.xml"));

    SerializerErrorReporterStream err(std::cerr);
    
    ModuleLoaderNodeJSPath loader;
    NodeModule & nodeModule = *loader.newModule("anonymous");
    bool rc = ser.read(
	nodeModule,
	in,
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

    std::ifstream in(testFilename("Test-xml-Serializer-parse-error.xml"));

    SerializerErrorReporterByCode err;
    ModuleLoaderNodeJSPath loader;
    NodeModule & nodeModule = *loader.newModule("anonymous");
    bool rc = ser.read(
	nodeModule,
	in,
	err);
    CHECK(!rc);
    CHECK(err.size() == 1);
    CHECK(err.getErrors(SerializerXML::ERROR_XML_PARSE).size() == 1);

}

TEST_CASE("test_xml_Serializer xml parse error with stream reporting", "[NodeJS][xml][Serializer]")
{
    const auto & ser = SerializerXML::instance();

    std::ifstream in(testFilename("Test-xml-Serializer-parse-error.xml"));

    std::ostringstream ostring;
    SerializerErrorReporterStream err(ostring);
    ModuleLoaderNodeJSPath loader;
    NodeModule & nodeModule = *loader.newModule("anonymous");
    bool rc = ser.read(
	nodeModule,
	in,
	err);
    CHECK(!rc);

    std::string ostr = ostring.str();
    CHECK(ostr.find("Failed to parse xml document") != std::string::npos);
}

