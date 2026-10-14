#include <catch2/catch_all.hpp>

#include "node--js/QualifiedIdentifier.hpp"

using namespace NodeJS::core;

TEST_CASE("test_QualifiedIdentifier_general_tests", "[NodeJS][core][NodeModule]")
{
    QualifiedIdentifier localName("some-local-node-type");
    CHECK(!localName.isQualified());
    CHECK(localName.getName() == "some-local-node-type");

    QualifiedIdentifier scopedName("org.ensor.foo");
    CHECK(scopedName.isQualified());
    CHECK(scopedName.getName() == "foo");
    CHECK(scopedName.getNamespace() == "org.ensor");

    // Now some edge cases with too few characters
    // to make sense.
    QualifiedIdentifier emptyNamespace(".foo");
    CHECK(emptyNamespace.isQualified());
    CHECK(emptyNamespace.getName() == "foo");
    CHECK(emptyNamespace.getNamespace() == "");

    QualifiedIdentifier emptyName("org.ensor.");
    CHECK(emptyName.isQualified());
    CHECK(emptyName.getName() == "");
    CHECK(emptyName.getNamespace() == "org.ensor");

    QualifiedIdentifier emptyEverything(".");
    CHECK(emptyEverything.isQualified());
    CHECK(emptyEverything.getName() == "");
    CHECK(emptyEverything.getNamespace() == "");

    QualifiedIdentifier noName("");
    CHECK(!noName.isQualified());
    CHECK(noName.getName() == "");
    CHECK(noName.getNamespace() == "");
}
