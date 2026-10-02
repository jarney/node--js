#include <catch2/catch_all.hpp>

#include "node--js/Validator.hpp"
#include "node--js/NodeModule.hpp"

using namespace NodeJS::core;

class Reporter : public ValidationError {
    virtual void reportError(ErrorType type, std::string message);
};

void
Reporter::reportError(ErrorType type, std::string message)
{
}


TEST_CASE("Validator", "[NodeJS][core][Validator]")
{
    Reporter err;
    NodeModule module;
    validate(module, err);
}
