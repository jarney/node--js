import {DataType} from "../modules/DataType.js";

var test = unitjs;

describe('DataType', function(){
    it('test_DataType_equality', function(){
	var dataType1 = new DataType("int", "Integer");
	var dataType2 = new DataType("int", "Another integer");
	
	test.assert(DataType.equals(dataType1, dataType2));
	
	// Things may compare equal as long as the ID is the same.
	// even if they have different names.
	test.assert(dataType1.getName() != dataType2.getName());
    }); 
});
