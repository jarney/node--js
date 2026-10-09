#include <catch2/catch_all.hpp>

#include "node--js/Metadata.hpp"
#include "node--js/ConnectionData.hpp"

using namespace NodeJS::core;

TEST_CASE("test_Metadata_equality", "[NodeJS][core][Metadata]")
{
    Metadata metadata;
    const Metadata & cmetadata = metadata;

    CHECK(!metadata.hasMetadata("some-entry"));
    CHECK(!cmetadata.hasMetadata("some-entry"));

    const ConnectionData & empty = cmetadata.getMetadata("some-entry");
    CHECK(empty.getValue("a") == "");
    
    // Retrieving a non-existent entry should create it.
    ConnectionData & cd = metadata.getMetadata("some-entry");
    cd.setValue("a", "b");
    CHECK(cd.getValue("a") == "b");
	
    const ConnectionData & cdconst = metadata.getMetadata("some-entry");
    CHECK(cdconst.getValue("a") == "b");

    // Check that when we get it again, it already exists and has our data.
    CHECK(metadata.hasMetadata("some-entry"));
    const ConnectionData & cd2 = metadata.getMetadata("some-entry");
    CHECK(cd2.getValue("a") == "b");

    CHECK(metadata.getNamespaces() == std::vector<std::string>{"some-entry"});
}
