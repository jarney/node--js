#include <catch2/catch_all.hpp>

#include "node--js/NamedPorts.hpp"
#include <limits.h>

using namespace NodeJS::core;

static const char *DATA_TYPE = "variable";

TEST_CASE("test_NamedPorts_features ", "[NodeJS][core][DataType]")
{
    NamedPorts namedPort;

    namedPort.addPort("a", std::make_unique<NodePort>(DATA_TYPE, "adesc", NodePort::ConnectionPolicy::Multiple));
    namedPort.addPort("b", std::make_unique<NodePort>(DATA_TYPE, "bdesc", NodePort::ConnectionPolicy::Multiple));
    namedPort.addPort("c", std::make_unique<NodePort>(DATA_TYPE, "cdesc", NodePort::ConnectionPolicy::Multiple));
    namedPort.addPort("d", std::make_unique<NodePort>(DATA_TYPE, "ddesc", NodePort::ConnectionPolicy::Multiple));

    CHECK(namedPort.hasPort("a"));
    CHECK(namedPort.hasPort("b"));
    CHECK(namedPort.hasPort("c"));
    CHECK(namedPort.hasPort("d"));

    CHECK(!namedPort.hasPort("z"));

    const NodePort *na = namedPort.getByName("a");
    CHECK(na != nullptr);
    CHECK(namedPort.getByName("a")->getDescription() == "adesc");
    CHECK(namedPort.getByName("b")->getDescription() == "bdesc");
    CHECK(namedPort.getByName("c")->getDescription() == "cdesc");
    CHECK(namedPort.getByName("d")->getDescription() == "ddesc");

    const NodePort *nnon = namedPort.getByName("z");
    CHECK(nnon == nullptr);

    CHECK(namedPort.getPortIndex("a") == 0);
    CHECK(namedPort.getPortIndex("b") == 1);
    CHECK(namedPort.getPortIndex("c") == 2);
    CHECK(namedPort.getPortIndex("d") == 3);
    CHECK(namedPort.getPortIndex("z") == INT_MAX);

    CHECK(namedPort.getCount() == 4);
    CHECK(namedPort.getByIndex(0)->getDescription() == "adesc");
    CHECK(namedPort.getByIndex(1)->getDescription() == "bdesc");
    CHECK(namedPort.getByIndex(2)->getDescription() == "cdesc");
    CHECK(namedPort.getByIndex(3)->getDescription() == "ddesc");
    CHECK(namedPort.getByIndex(4) == nullptr);

    CHECK(namedPort.getName(0) == "a");
    CHECK(namedPort.getName(1) == "b");
    CHECK(namedPort.getName(2) == "c");
    CHECK(namedPort.getName(3) == "d");

}
