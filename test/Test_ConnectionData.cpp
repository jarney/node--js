#include <catch2/catch_all.hpp>

#include "node--js/ConnectionData.hpp"

using namespace NodeJS::core;

TEST_CASE("ConnectionData basics", "[NodeJS][core][ConnectionData]")
{
    ConnectionData cd;
}

TEST_CASE("ConnectionData default values", "[NodeJS][core][ConnectionData]")
{
    ConnectionData cd;

    CHECK(!cd.hasValue("unknown-value"));
    
    // If we specify a default, check that it's honored.
    CHECK(cd.getValue("unknown-value", "some-default") == "some-default");

    // If we don't specify a default, it's the empty string.
    CHECK(cd.getValue("unknown-value") == "");
}

TEST_CASE("ConnectionData actual values", "[NodeJS][core][ConnectionData]")
{
    ConnectionData cd;

    // If we actually have a value, make sure it works.
    cd.setValue("well-known-value", "actual value");
    
    CHECK(cd.hasValue("well-known-value"));
    CHECK(cd.getValue("well-known-value", "other-default") == "actual value");

}

TEST_CASE("ConnectionData copy", "[NodeJS][core][ConnectionData]")
{
    ConnectionData cd;
    // If we actually have a value, make sure it works.
    cd.setValue("well-known-value", "actual value");

    // Check that whatever we do to one,
    // is done to the other.
    ConnectionData cdOther(cd);
    cdOther.setValue("only-other-value", "blue");

    CHECK(cdOther.hasValue("well-known-value"));
    CHECK(cdOther.getValue("well-known-value", "other-default") == "actual value");

    // Check that the new value ONLY shows up on the new list.
    CHECK(!cd.hasValue("only-other-value"));
    CHECK(cdOther.hasValue("only-other-value"));

}

