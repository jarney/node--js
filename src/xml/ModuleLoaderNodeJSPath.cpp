#include "node--js/xml/ModuleLoaderNodeJSPath.hpp"
#include "node--js/NodeModule.hpp"
#include <string>
#include <iostream>

using namespace NodeJS::core;
using namespace NodeJS::xml;

static std::vector<std::string>
split_path(std::string str)
{
    std::vector<std::string> path;
    for (std::string::const_iterator c = str.cbegin(); c != str.cend(); c++) {
	std::cout << "Character " << *c << std::endl;
    }
    return path;
}

void
ModuleLoaderNodeJSPath::setNODEJS_PATH(std::string path)
{
    mPath = split_path(path);
}

const NodeModule *
ModuleLoaderNodeJSPath::loadModule(
    std::string aFullyQualifiedModuleName,
    SerializerErrorReporter & reporter
    )
{
    fprintf(stderr, "Loading a module...\n");
    return nullptr;
}

