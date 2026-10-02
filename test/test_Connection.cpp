#include <catch2/catch_all.hpp>

#include "node--js/Connection.hpp"

using namespace NodeJS::core;

TEST_CASE("Connection equality", "[NodeJS][core][Connection]")
{
    Connection conn0{"n0", "p0", "n1", "p1"};
    // Only one way that they can be the same.
    Connection conn1{"n0", "p0", "n1", "p1"};
    CHECK(conn0 == conn1);

    // Lots of different ways it can be different.
    Connection conn2{"n0", "p1", "n1", "p1"};
    Connection conn3{"n1", "p0", "n1", "p1"};
    Connection conn4{"n0", "p0", "n2", "p1"};
    Connection conn5{"n0", "p0", "n1", "p2"};

    CHECK(conn0 != conn2);
    CHECK(conn0 != conn3);
    CHECK(conn0 != conn4);
    CHECK(conn0 != conn5);
}

TEST_CASE("Connection id", "[NodeJS][core][Connection]")
{
    Connection conn0{"n0", "p0", "n1", "p1"};
    CHECK(conn0.getId() == "n0-p0|n1-p1");
}
