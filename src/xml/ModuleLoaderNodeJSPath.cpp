#include "node--js/xml/ModuleLoaderNodeJSPath.hpp"
#include "node--js/NodeModule.hpp"
#include "node--js/xml/SerializerXML.hpp"
#include <string>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <fstream>

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
    }
    if (state == ESCAPE) {
	os << '\\';
    }
    if (os.str().size() > 0) {
	path.push_back(os.str());
    }
    return path;
}

ModuleLoaderNodeJSPath::ModuleLoaderNodeJSPath()
{
    const char *nodejs_path = getenv("NODEJS_PATH");
    if (nodejs_path != nullptr) {
	mPath = split_path(std::string(nodejs_path));
    }
    else {
	mPath.push_back(".");
    }
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
    
    const SerializerXML & ser = SerializerXML::instance();
    for (const auto & pathElement : mPath) {
	std::string filename = aFullyQualifiedModuleName + std::string(".xml");
	std::filesystem::path full_filename = std::filesystem::path(pathElement) / filename;
	std::unique_ptr<NodeModule> module = std::make_unique<NodeModule>();
	std::ifstream input_stream(full_filename);
	bool success = ser.read(
	    *module,
	    input_stream,
	    reporter
	    );
	if (!success) {
	    module = nullptr;
	}
	// Even if we fail, we want to put the (null) module
	// onto the list so we don't try (and fail) to load it again.
	const NodeModule *ret_module = module.get();
	mLoadedModules.insert(std::make_pair(aFullyQualifiedModuleName, std::move(module)));
	return ret_module;
    }
    return nullptr;
}

