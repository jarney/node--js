#include <catch2/catch_all.hpp>
#include <iostream>
#include <fstream>

#include "node--js/xml/Serializer.hpp"
#include "TestData.h"

using namespace NodeJS::core;
using namespace NodeJS::xml;
using namespace NodeJS::test;

TEST_CASE("xml::Serializer", "[NodeJS][xml][Serializer]")
{
    const auto & ser = ::NodeJS::xml::Serializer::instance();

    std::ifstream in(testFilename("Test-Serializer-Basic.xml"));
    
    NodeModule nodeModule;
    bool rc = ser.read(
	nodeModule,
	in,
	std::cerr);
    CHECK(rc);

    rc = ser.write(
	nodeModule,
	std::cout,
	std::cerr);
    CHECK(rc);
}
