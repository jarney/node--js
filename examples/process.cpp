#include <iostream>
#include <fstream>

#include <boost/program_options.hpp>

#include "node--js/NodeModule.hpp"
#include "node--js/ModuleLoader.hpp"
#include "node--js/xml/SerializerXML.hpp"
#include "node--js/xml/ModuleLoaderNodeJSPath.hpp"
#include "node--js/SerializerError.hpp"
#include "node--js/Processor.hpp"
#include "node--js/Natives.hpp"

using namespace NodeJS::core;
using namespace NodeJS::xml;

namespace po = boost::program_options;

int usage(po::options_description & desc)
{
    std::cerr << desc << "\n";
    std::cerr << "  This program reads a node module and executes it\n";
    std::cerr << "  and produces the output from execution\n";
    return 1;
}

static std::pair<std::string, std::string>
split(const std::string& s, char c) {
    std::string name;
    std::string value;

    size_t begin = 0;
    size_t end = s.find_first_of(c, begin);
    
    name = s.substr(begin, end - begin);
    if (end != std::string::npos) {
	value = s.substr(end+1, std::string::npos);
    }
    return std::make_pair(name, value);
}

static
std::vector<std::pair<std::string, std::string>>
parse_args(const std::vector<std::string> & args)
{
    std::vector<std::pair<std::string, std::string>> ret;
    for (const auto & str : args) {
	ret.push_back(split(str, '='));
    }
    return ret;
}


int main(int argc, char **argv)
{
    po::options_description desc("Usage: process [options] module-name");
    desc.add_options()
	("module-name", po::value<std::string>(), "Module to load")
	("entrypoint", po::value<std::string>(), "Entrypoint (graph-id)")
	("arg", po::value<std::vector<std::string>>(), "Define a value")
	("help", "produce help message")
	;
    po::positional_options_description p;
    p.add("module-name", -1);

    // Declare the supported options.
    po::variables_map vm;
    try {
	po::store(po::command_line_parser(argc, argv)
		  .options(desc)
		  .positional(p)
		  .run(), vm);
	po::notify(vm);    
    }
    catch (boost::program_options::error & ex) {
	std::cerr << ex.what() << "\n";
	return usage(desc);
    }
    
    if (vm.count("help")) {
	return usage(desc);
    }

    if (vm.count("module-name") != 1) {
	return usage(desc);
    }
    std::string entrypoint = "main";
    if (vm.count("entrypoint") != 0) {
	entrypoint = vm["entrypoint"].as<std::string>();
    }

    const std::string & moduleName = vm["module-name"].as<std::string>();

    fprintf(stderr, "Processing module %s : %s\n", moduleName.c_str(), entrypoint.c_str());

    const SerializerXML & ser = SerializerXML::instance();
    auto pathLoader = std::make_unique<ModuleLoaderNodeJSPath>();
    ModuleLoader loader(std::move(pathLoader));
    SerializerErrorReporterStream err(std::cerr);

    fprintf(stderr, "Loading module\n");
    NodeModule *nodeModule = loader.loadModule(moduleName, err);
    fprintf(stderr, "Loaded module\n");
    if (!nodeModule) {
	fprintf(stderr, "No such module found.  Please check NODEJS_PATH\n");
	return 1;
    }

    Processor processor;
    
    ConnectionData input;
    if (vm.count("arg") != 0) {
	const std::vector<std::string> & args = vm["arg"].as<std::vector<std::string>>();
	const auto & args_parsed = parse_args(args);
	
	for (const auto & arg : args_parsed) {
	    input.setValue(arg.first, arg.second);
	}
    }
    //input.setValue("inval", "Input value from graph");

    ConnectionData output;
    NodeGraph *graph = nodeModule->getGraph(entrypoint);
    if (!graph) {
	fprintf(stderr, "Graph 'main' not found in module '%s'\n", entrypoint.c_str());
	return 2;
    }
    
    processor.setNativeImpl(NodeProcessor_string::ID, std::make_unique<NodeProcessor_string>());
    processor.setNativeImpl(NodeProcessor_concatenate::ID, std::make_unique<NodeProcessor_concatenate>());
    processor.setNativeImpl(NodeProcessor_input::ID, std::make_unique<NodeProcessor_input>());
    processor.setNativeImpl(NodeProcessor_output::ID, std::make_unique<NodeProcessor_output>());

    processor.processGraph(*graph, input, output);

    std::string output_str = output.getValue("out");
    fwrite(output_str.c_str(), output_str.size(), 1, stdout);
    fflush(stdout);
    
    return 0;
}

