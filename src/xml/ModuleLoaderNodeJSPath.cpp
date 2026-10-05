#include "node--js/xml/ModuleLoaderNodeJSPath.hpp"
#include "node--js/NodeModule.hpp"
#include <string>
#include <sstream>
#include <iostream>

using namespace NodeJS::core;
using namespace NodeJS::xml;

static const int NORMAL = 0;
static const int ESCAPE = 1;

static std::vector<std::string>
split_path(std::string str)
{
    std::ostringstream os;
    std::vector<std::string> path;
    int state = NORMAL;
    for (std::string::const_iterator it = str.cbegin(); it != str.cend(); it++) {
	char c = *it;
	if (state == NORMAL) {
	    if (c == '\\') {
		state = ESCAPE;
	    }
	    else if (c == ';') {
		path.push_back(os.str());
		os.str("");
		os.clear();
		state = NORMAL;
	    }
	    else {
		os << c;
	    }
	}
	else /*if (state == ESCAPE)*/ {
	    if (c == ';') {
		os << c;
		state = NORMAL;
	    }
	    else if (c == '\\') {
		os << "\\\\";
	    }
	    else {
		os << '\\' << c;
	    }
	    state = NORMAL;
	}
	std::cout << "Character " << c << std::endl;
    }
    if (state == ESCAPE) {
	os << '\\';
    }
    if (os.str().size() > 0) {
	path.push_back(os.str());
    }
    return path;
}

void
ModuleLoaderNodeJSPath::setNODEJS_PATH(std::string path)
{
    mPath = split_path(path);
}

const std::vector<std::string> &
ModuleLoaderNodeJSPath::getNODEJS_PATH()
{
    return mPath;
}

const NodeModule *
ModuleLoaderNodeJSPath::loadModule(
    std::string aFullyQualifiedModuleName,
    SerializerErrorReporter & reporter
    )
{
    const auto & it = mLoadedModules.find(aFullyQualifiedModuleName);
    if (it != mLoadedModules.end()) {
	return it->second.get();
    }
    
    NodeJS::xml::Serializer ser = NodeJS::xml::Serializer::instance();
    for (const auto & pathElement : mPath) {
// Construct filename based on path element and .xml extension.
//      file = pathElement + "/" + aFullyQualifiedModuleName + ".xml";
//	if (exists(file)) {
	// std::unique_ptr<NodeModule> module = std::make_unique<NodeModule>();
	// ser.read(pathElement, reporter);
	// if (!success) {
	//     module = nullptr;
	// }
	// mLoadedModules.insert(std::make_pair(aFullyQualifiedModuleName, std::move(module)));

    }
    return nullptr;
}

