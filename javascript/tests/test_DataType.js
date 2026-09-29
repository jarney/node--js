import {UnitTests} from "./UnitTests.js";
import {DataType} from "../modules/DataType.js";

UnitTests.TEST_CASE("test_DataType_equality", function () {
    var dataType1 = new DataType("int", "Integer");
    var dataType2 = new DataType("int", "Another integer");
    
    UnitTests.CHECK(DataType.equals(dataType1, dataType2), "test_DataType_equality ==");
    
    // Things may compare equal as long as the ID is the same.
    // even if they have different names.
    UnitTests.CHECK(dataType1.getName() != dataType2.getName(), "test_DataType_equality !=");
});

