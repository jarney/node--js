#include <iostream>
#include <fstream>
#include "node--js/NodeModule.hpp"
#include "node--js/xml/Serializer.hpp"

using namespace NodeJS::core;

int main(int argc, char **argv)
{
    const Serializer & ser = NodeJS::xml::Serializer::instance();

    NodeModule mod;

    std::string fname(argv[1]);
    std::ifstream input_stream(fname);
    
    ser.read(mod, input_stream, std::cerr);
    ser.write(mod, std::cout, std::cerr);
    
    return 0;
}

