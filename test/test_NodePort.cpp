#include <catch2/catch_all.hpp>

#include "node--js/NodePort.hpp"

using namespace NodeJS::core;

TEST_CASE("test_NodePort_multiple", "[NodeJS][core][NodePort]")
{
    const char *DATA_TYPE = "variable";
    const char *DESCRIPTION = "Input A";
    NodePort nodePort(DATA_TYPE, DESCRIPTION, NodePort::ConnectionPolicy::Multiple);

    CHECK(nodePort.getDataType() == DATA_TYPE);
    CHECK(nodePort.getDescription() == DESCRIPTION);
    CHECK(nodePort.getConnectionPolicy() == NodePort::ConnectionPolicy::Multiple);

}

TEST_CASE("test_NodePort_default", "[NodeJS][core][NodePort]")
{
    const char *DATA_TYPE = "variable";
    const char *DESCRIPTION = "Input A";
    NodePort nodePort(DATA_TYPE, DESCRIPTION);

    CHECK(nodePort.getDataType() == DATA_TYPE);
    CHECK(nodePort.getDescription() == DESCRIPTION);
    CHECK(nodePort.getConnectionPolicy() == NodePort::ConnectionPolicy::One);

}

TEST_CASE("test_NodePort_explicit_one", "[NodeJS][core][NodePort]")
{
    const char *DATA_TYPE = "variable";
    const char *DESCRIPTION = "Input A";
    NodePort nodePort(DATA_TYPE, DESCRIPTION, NodePort::ConnectionPolicy::One);

    CHECK(nodePort.getDataType() == DATA_TYPE);
    CHECK(nodePort.getDescription() == DESCRIPTION);
    CHECK(nodePort.getConnectionPolicy() == NodePort::ConnectionPolicy::One);

}

TEST_CASE("test_NodePort_specific_metadata", "[NodeJS][core][NodeGraph][Node]")
{
    const char *DATA_TYPE = "variable";
    const char *DESCRIPTION = "Input A";
    NodePort nodePort(DATA_TYPE, DESCRIPTION);

    Metadata & metadata = nodePort.getMetadata();

    // Check that we don't have any specific metadata yet.
    CHECK(!metadata.hasMetadata("foo.bar.org"));

    // Check that when we ask for metadata, it's created
    ConnectionData & cd = metadata.getMetadata("foo.bar.org");
    CHECK(metadata.hasMetadata("foo.bar.org"));

    cd.setValue("x", "some-value");

    // Check that we can make a const-version of this.
    const NodePort & constNodePort = nodePort;
    const Metadata & constMetadata = constNodePort.getMetadata();
    const ConnectionData & constData = constMetadata.getMetadata("foo.bar.org");
    CHECK(constData.hasValue("x"));

    const ConnectionData & readonly = constMetadata.getMetadata("some-other-namespace");
    // Check that in the const context, we didn't actually add the namespace,
    // just faked it by returning an empty connection data.
    CHECK(!constMetadata.hasMetadata("some-other-namespace"));
    CHECK(!readonly.hasValue("x"));
}
