#include <catch2/catch_all.hpp>

//#include "node--js/xml/Serializer.hpp"
#include "node--js/xml/ModuleLoaderNodeJSPath.hpp"
#include "node--js/SerializerError.hpp"
#include "TestData.h"

using namespace NodeJS::core;
using namespace NodeJS::xml;
using namespace NodeJS::test;

void doTest(std::string given_path, const std::vector<std::string> & expected)
{
    ModuleLoaderNodeJSPath moduleLoader;
    moduleLoader.setNODEJS_PATH(given_path);
    const auto & path = moduleLoader.getNODEJS_PATH();
    for (const auto & it : path) {
	fprintf(stderr, "%s\n", it.c_str());
    }
    
//    std::vector<std::string> expected{"one", "two", "thre;e", "four"};
    CHECK(path == expected);
}

TEST_CASE("xml::ModuleLoaderNodeJSPath path splitting: ordinary", "[NodeJS][xml][ModuleLoader]")
{
    doTest("one;two;three;four", std::vector<std::string>{"one", "two", "three", "four"});
}

TEST_CASE("xml::ModuleLoaderNodeJSPath path splitting: escape semicolon", "[NodeJS][xml][ModuleLoader]")
{
    doTest("one;two;thre\\;e;four", std::vector<std::string>{"one", "two", "thre;e", "four"});
}

TEST_CASE("xml::ModuleLoaderNodeJSPath path splitting: Just a stray escape", "[NodeJS][xml][ModuleLoader]")
{
    doTest("\\", std::vector<std::string>{"\\"});
}

TEST_CASE("xml::ModuleLoaderNodeJSPath path splitting: double escape backslash", "[NodeJS][xml][ModuleLoader]")
{
    doTest("\\\\", std::vector<std::string>{"\\\\"});
    doTest("a\\\\b", std::vector<std::string>{"a\\\\b"});
}

TEST_CASE("xml::ModuleLoaderNodeJSPath path splitting: escape other things", "[NodeJS][xml][ModuleLoader]")
{
    doTest("\\a", std::vector<std::string>{"\\a"});
    doTest("\\a;\\c", std::vector<std::string>{"\\a", "\\c"});
}

TEST_CASE("xml::ModuleLoaderNodeJSPath path splitting: empty", "[NodeJS][xml][ModuleLoader]")
{
    doTest("", std::vector<std::string>{});
}
