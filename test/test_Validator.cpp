#include <catch2/catch_all.hpp>

#include "node--js/Validator.hpp"
#include "node--js/NodeModule.hpp"
#include "node--js/xml/ModuleLoaderNodeJSPath.hpp"

using namespace NodeJS::core;
using namespace NodeJS::xml;

class Reporter : public ValidationError {
    virtual void reportError(ErrorType type, std::string message);
};

void
Reporter::reportError(ErrorType type, std::string message)
{
}


TEST_CASE("test_Validator_basic", "[NodeJS][core][Validator]")
{
    Reporter err;
    ModuleLoaderNodeJSPath loader;
    NodeModule & module = *loader.newModule("anonymous");
    validate(module, err);
}
