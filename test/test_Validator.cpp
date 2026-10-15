#include <catch2/catch_all.hpp>

#include "node--js/Validator.hpp"
#include "node--js/NodeModule.hpp"
#include "node--js/ModuleLoader.hpp"

using namespace NodeJS::core;

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
    ModuleLoader loader;
    auto nodeModule = loader.newModule("anonymous");
    validate(*nodeModule, err);
}
