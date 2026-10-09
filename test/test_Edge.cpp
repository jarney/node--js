#include <catch2/catch_all.hpp>

#include "node--js/Edge.hpp"

using namespace NodeJS::core;

TEST_CASE("test_Edge_equality", "[NodeJS][core][Edge]")
{
    Edge conn0{"n0", "p0", "n1", "p1"};
    // Only one way that they can be the same.
    Edge conn1{"n0", "p0", "n1", "p1"};
    CHECK(conn0 == conn1);

    // Lots of different ways it can be different.
    Edge conn2{"n0", "p1", "n1", "p1"};
    Edge conn3{"n1", "p0", "n1", "p1"};
    Edge conn4{"n0", "p0", "n2", "p1"};
    Edge conn5{"n0", "p0", "n1", "p2"};

    CHECK(conn0 != conn2);
    CHECK(conn0 != conn3);
    CHECK(conn0 != conn4);
    CHECK(conn0 != conn5);
}

TEST_CASE("test_Edge_id", "[NodeJS][core][Edge]")
{
    Edge conn0{"n0", "p0", "n1", "p1"};
    CHECK(conn0.getId() == "n0-p0|n1-p1");
}
