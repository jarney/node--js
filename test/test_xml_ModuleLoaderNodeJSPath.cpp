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

TEST_CASE("test_xml_ModuleLoaderNodeJSPath path splitting: ordinary", "[NodeJS][xml][ModuleLoader]")
{
    doTest(
	std::string("one") + NODEJS_PATH_DELIMITER_STRING + 
	std::string("two") + NODEJS_PATH_DELIMITER_STRING + 
	std::string("three") + NODEJS_PATH_DELIMITER_STRING + 
	std::string("four"),
	std::vector<std::string>{"one", "two", "three", "four"}
	);
}

TEST_CASE("test_xml_ModuleLoaderNodeJSPath path splitting: escape semicolon", "[NodeJS][xml][ModuleLoader]")
{
    doTest(
	std::string("one") + NODEJS_PATH_DELIMITER_STRING + 
	std::string("two") + NODEJS_PATH_DELIMITER_STRING + 
	std::string("thre") + NODEJS_PATH_ESCAPE_STRING + NODEJS_PATH_DELIMITER_STRING + std::string("e") + NODEJS_PATH_DELIMITER_STRING + 
	std::string("four"),
	std::vector<std::string>{"one", "two", std::string("thre") + NODEJS_PATH_DELIMITER_STRING + std::string("e"), "four"});
}

TEST_CASE("test_xml_ModuleLoaderNodeJSPath path splitting: Just a stray escape", "[NodeJS][xml][ModuleLoader]")
{
    doTest(NODEJS_PATH_ESCAPE_STRING, std::vector<std::string>{NODEJS_PATH_ESCAPE_STRING});
}

TEST_CASE("test_xml_ModuleLoaderNodeJSPath path splitting: double escape backslash", "[NodeJS][xml][ModuleLoader]")
{
    doTest(NODEJS_PATH_ESCAPE_STRING + NODEJS_PATH_ESCAPE_STRING, std::vector<std::string>{NODEJS_PATH_ESCAPE_STRING + NODEJS_PATH_ESCAPE_STRING});
    doTest(
	std::string("a") +
	NODEJS_PATH_ESCAPE_STRING + NODEJS_PATH_ESCAPE_STRING +
	std::string("b"),
	std::vector<std::string>{
	    std::string("a") +
	    NODEJS_PATH_ESCAPE_STRING + NODEJS_PATH_ESCAPE_STRING +
	    std::string("b"),
	});
}

TEST_CASE("test_xml_ModuleLoaderNodeJSPath path splitting: escape other things", "[NodeJS][xml][ModuleLoader]")
{
    doTest(
	NODEJS_PATH_ESCAPE_STRING + std::string("a"),
	std::vector<std::string>{
	    NODEJS_PATH_ESCAPE_STRING + std::string("a")
	}
	);
    doTest(
	NODEJS_PATH_ESCAPE_STRING + std::string("a") +
	NODEJS_PATH_DELIMITER_STRING +
	NODEJS_PATH_ESCAPE_STRING + std::string("c"),
	std::vector<std::string>{
	    NODEJS_PATH_ESCAPE_STRING + std::string("a"),
	    NODEJS_PATH_ESCAPE_STRING + std::string("c")
	}
	);
}

TEST_CASE("test_xml_ModuleLoaderNodeJSPath path splitting: empty", "[NodeJS][xml][ModuleLoader]")
{
    doTest("", std::vector<std::string>{});
}

TEST_CASE("test_xml_ModuleLoaderNodeJSPath setting path and loading file", "[NodeJS][xml][ModuleLoader]")
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

TEST_CASE("test_xml_ModuleLoaderNodeJSPath test environment variable", "[NodeJS][xml][ModuleLoader]")
{
    std::string pathstr =
	std::string("a") +
	NODEJS_PATH_DELIMITER_STRING +
	std::string("b");

    // Force the resolution to use the environment variable.
    unsetenv("NODEJS_PATH");
    setenv("NODEJS_PATH", pathstr.c_str(), 1);
    ModuleLoaderNodeJSPath loader;

    CHECK(loader.getNODEJS_PATH() == std::vector<std::string>{"a", "b"});
}

TEST_CASE("test_xml_ModuleLoaderNodeJSPath new module is same", "[NodeJS][xml][ModuleLoader]")
{
    ModuleLoaderNodeJSPath loader;

    NodeModule *newModule = loader.newModule("unique-module-name");

    NodeModule *nextModule = loader.newModule("unique-module-name");

    // Module names are unique.  If we ask for a new one, we
    // should expect to get the same one.
    CHECK(newModule == nextModule);
}
