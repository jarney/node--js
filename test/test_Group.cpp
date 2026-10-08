#include <catch2/catch_all.hpp>

#include "node--js/Group.hpp"

using namespace NodeJS::core;

TEST_CASE("Group check basic stuff", "[NodeJS][core][NodeGraph][Node]")
{
    Group g;

    // Add node, make sure we can find it.
    g.addNode("foo");
    CHECK(g.contains("foo"));
    CHECK(g.getNodes() == std::set<std::string>{"foo"});

    // Remove node, make sure it goes away.
    g.removeNode("foo");
    CHECK(!g.contains("foo"));

    // Make sure non-existing node isnt there.
    CHECK(!g.contains("bar"));
    g.removeNode("bar");
    CHECK(!g.contains("bar"));

    CHECK(g.getNodes() == std::set<std::string>());
}
