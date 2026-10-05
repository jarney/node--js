#include <catch2/catch_all.hpp>

//#include "node--js/xml/Serializer.hpp"
#include "node--js/xml/ModuleLoaderNodeJSPath.hpp"
#include "node--js/SerializerError.hpp"
#include "TestData.h"

using namespace NodeJS::core;
using namespace NodeJS::xml;
using namespace NodeJS::test;

TEST_CASE("xml::ModuleLoaderNodeJSPath path splitting", "[NodeJS][xml][ModuleLoader]")
{
    ModuleLoaderNodeJSPath moduleLoader;

    moduleLoader.setNODEJS_PATH("one;two;thre\\;e;four");
}
