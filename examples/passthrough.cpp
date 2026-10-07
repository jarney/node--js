#include <iostream>
#include <fstream>
#include "node--js/NodeModule.hpp"
#include "node--js/ModuleLoader.hpp"
#include "node--js/xml/SerializerXML.hpp"
#include "node--js/xml/ModuleLoaderNodeJSPath.hpp"
#include "node--js/SerializerError.hpp"

using namespace NodeJS::core;
using namespace NodeJS::xml;

int main(int argc, char **argv)
{
    if (argc != 2) {
	fprintf(stderr, "Usage: passthrough module-name\n");
	fprintf(stderr, "    This reads a class file and dumps it back out to verify\n");
	fprintf(stderr, "    the serializer and make sure we can read and write all\n");
	fprintf(stderr, "    important information in the file\n");
	return -1;
    }
    
    const SerializerXML & ser = SerializerXML::instance();

    ModuleLoaderNodeJSPath loader;
    SerializerErrorReporterStream err(std::cerr);
    
    NodeModule *mod = loader.loadModule(std::string(argv[1]), err);
    if (!mod) {
	fprintf(stderr, "No such module found.  Please check NODEJS_PATH\n");
	return 1;
    }
    
    ser.write(*mod, std::cout, err);
    
    return 0;
}

