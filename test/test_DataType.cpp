#include <catch2/catch_all.hpp>

#include "node--js/DataType.hpp"

using namespace NodeJS::core;

TEST_CASE("DataType equality", "[NodeJS][core][DataType]")
{
    DataType dataType1("int", "Integer");
    DataType dataType2("int", "Another integer");
    
    CHECK(dataType1 == dataType2);
    CHECK(!(dataType1 != dataType2));

    // Things may compare equal as long as the ID is the same.
    // even if they have different names.
    CHECK(dataType1.getName() != dataType2.getName());
    
}

TEST_CASE("DataType copy", "[NodeJS][core][DataType]")
{
    DataType dataType1("int", "Integer");
    DataType dataType2(dataType1);
    
    CHECK(dataType1 == dataType2);

    // Things may compare equal as long as the ID is the same.
    // even if they have different names.
    CHECK(dataType1.getName() == dataType2.getName());
    
}

TEST_CASE("DataType assign", "[NodeJS][core][DataType]")
{
    DataType dataType1("int", "Integer");
    DataType dataType2("float", "Float");

    dataType2 = dataType1;

    // After assignment, these two objects
    // should compare equal in all ways.
    CHECK(dataType1 == dataType2);
    CHECK(dataType1.getId() == dataType2.getId());
    CHECK(dataType1.getName() == dataType2.getName());
    
}
