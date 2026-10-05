#include <catch2/catch_all.hpp>
#include <iostream>

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

TEST_CASE("xml::ModuleLoaderNodeJSPath setting path and loading file", "[NodeJS][xml][ModuleLoader]")
{
    ModuleLoaderNodeJSPath loader;
    SerializerErrorReporterStream err(std::cerr);

    std::string path = testDirectory();
    loader.setNODEJS_PATH(path);
    const NodeModule *module = loader.loadModule("org.ensor.nodejs.openscad", err);
    CHECK(module != nullptr);

    // Load it again.
    const NodeModule *module2 = loader.loadModule("org.ensor.nodejs.openscad", err);
    CHECK(module2 != nullptr);

    // Check that we actually got the same cached module.
    CHECK(module == module2);

    // Check that we get a null pointer
    // if the module doesn't exist.
    const NodeModule *module_nonexistent = loader.loadModule("some-nonexistent-module", err);
    CHECK(module_nonexistent == nullptr);
    
    loader.setNODEJS_PATH("");
    const NodeModule *module_no_path = loader.loadModule("package-with-no-path-or-chance-to-load", err);
    CHECK(module_no_path == nullptr);
}
