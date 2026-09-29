#include <catch2/catch_all.hpp>

#include "node--js/NodePort.hpp"

using namespace NodeJS::core;

TEST_CASE("NodePort multiple", "[NodeJS][NodePort]")
{
    const char *DATA_TYPE = "variable";
    const char *DESCRIPTION = "Input A";
    NodePort nodePort(DATA_TYPE, DESCRIPTION, NodePort::ConnectionPolicy::Multiple);

    CHECK(nodePort.getDataType() == DATA_TYPE);
    CHECK(nodePort.getDescription() == DESCRIPTION);
    CHECK(nodePort.getConnectionPolicy() == NodePort::ConnectionPolicy::Multiple);

}

TEST_CASE("NodePort default", "[NodeJS][NodePort]")
{
    const char *DATA_TYPE = "variable";
    const char *DESCRIPTION = "Input A";
    NodePort nodePort(DATA_TYPE, DESCRIPTION);

    CHECK(nodePort.getDataType() == DATA_TYPE);
    CHECK(nodePort.getDescription() == DESCRIPTION);
    CHECK(nodePort.getConnectionPolicy() == NodePort::ConnectionPolicy::One);

}

TEST_CASE("NodePort explicit one", "[NodeJS][NodePort]")
{
    const char *DATA_TYPE = "variable";
    const char *DESCRIPTION = "Input A";
    NodePort nodePort(DATA_TYPE, DESCRIPTION, NodePort::ConnectionPolicy::One);

    CHECK(nodePort.getDataType() == DATA_TYPE);
    CHECK(nodePort.getDescription() == DESCRIPTION);
    CHECK(nodePort.getConnectionPolicy() == NodePort::ConnectionPolicy::One);

}
