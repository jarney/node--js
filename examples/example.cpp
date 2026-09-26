#include <stdio.h>
#include <iostream>
#include "example.hpp"
//#include "Scope.hpp"

int main(int argc, char **argv)
{
//    Registry builtins;
//    builtins.registerType(Type{"sphere"});
//    builtins.registerType(Type{"cube"});
//    builtins.dump();
//
//    Scope graphScope;
    
    lib_main(argc, argv);
}

#if 0
Type::Type(std::string name)
    : _name(name)
{}

void
Type::dump()
{
    std::cout << _name << std::endl;
}
std::string
Type::getName(void)
{
    return _name;
}


void
Registry::registerType(Type t)
{
    _typeMap[t.getName()] = t;
}

void
Registry::dump(void)
{
    for (const auto & it : _typeMap) {
	std::cout << it.first << std::endl;
    }
}
#endif
