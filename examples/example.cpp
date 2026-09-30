#include <iostream>
#include <fstream>
#include "node--js/NodeModule.hpp"
#include "node--js/xml/Serializer.hpp"
#include "node--js/SerializerError.hpp"

using namespace NodeJS::core;

int main(int argc, char **argv)
{
    const Serializer & ser = NodeJS::xml::Serializer::instance();

    NodeModule mod;

    std::string fname(argv[1]);
    std::ifstream input_stream(fname);

    SerializerErrorReporterStream err(std::cerr);
    
    ser.read(mod, input_stream, err);
    ser.write(mod, std::cout, err);
    
    return 0;
}

