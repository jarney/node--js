#include <catch2/catch_all.hpp>

#include "node--js/ConnectionData.hpp"

using namespace NodeJS::core;

TEST_CASE("test_ConnectionData_constructor", "[NodeJS][core][ConnectionData]")
{
    ConnectionData cd;
}

TEST_CASE("test_ConnectionData_default_values", "[NodeJS][core][ConnectionData]")
{
    ConnectionData cd;

    CHECK(!cd.hasValue("unknown-value"));
    
    // If we specify a default, check that it's honored.
    CHECK(cd.getValue("unknown-value", "some-default") == "some-default");

    // If we don't specify a default, it's the empty string.
    CHECK(cd.getValue("unknown-value") == "");
}

TEST_CASE("test_ConnectionData_actual_values", "[NodeJS][core][ConnectionData]")
{
    ConnectionData cd;

    // If we actually have a value, make sure it works.
    cd.setValue("well-known-value", "actual value");
    
    CHECK(cd.hasValue("well-known-value"));
    CHECK(cd.getValue("well-known-value", "other-default") == "actual value");

}

TEST_CASE("test_ConnectionData_copy", "[NodeJS][core][ConnectionData]")
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

TEST_CASE("test_ConnectionData_data", "[NodeJS][core][ConnectionData]")
{
    ConnectionData cd;
    cd.setValue("well-known-value", "actual value");
    
    const auto & d = cd.getData();

    const auto & it = d.find("well-known-value");
    CHECK(it != d.end());
    CHECK(it->second == "actual value");
}

TEST_CASE("test_ConnectionData_append", "[NodeJS][core][ConnectionData]")
{
    ConnectionData cd;
    cd.setValue("well-known-value", "value-1");
    cd.appendValue("well-known-value", "value-2");
    CHECK(cd.getValue("well-known-value") == "value-1value-2");
}
