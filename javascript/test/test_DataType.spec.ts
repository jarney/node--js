import { describe, it, expect } from "vitest";

import { DataType } from "../src/DataType";

describe("DataType", function(){
    it("test_DataType_equality", function(){
      var dataType1 = new DataType("int", "pancake", "Integer");
      var dataType2 = new DataType("int", "pancake", "Another integer");
      
      expect(true).to.equal(DataType.equals(dataType1, dataType2));
      
      // Things may compare equal as long as the ID is the same.
      // even if they have different names.
      expect(dataType1.getName()).not.to.equal(dataType2.getName());
    });
  
  it("test_DataType_copy", function() {
    var dataType1 = new DataType("int", "pancake", "Integer");
    var dataType2 = new DataType("int", "pancake", "");
    
    // Things may compare equal as long as the ID is the same.
    // even if they have different names.
    expect(true).to.equal(DataType.equals(dataType1, dataType2));
    expect(dataType1.getName()).not.to.equal(dataType2.getName());
  });    
  
  it("test_DataType_assign", function() {
    var dataType1 = new DataType("int", "pancake", "Integer");
    var dataType2 = new DataType("float", "waffle", "Float");
    
    dataType2.copy(dataType1);
    
    // After assignment, these two objects
    // should compare equal in all ways.
    expect(true).to.equal(DataType.equals(dataType1, dataType2));
    expect(dataType1.getFullyQualifiedId()).to.equal(dataType2.getFullyQualifiedId());
    expect(dataType1.getName()).to.equal(dataType2.getName());
    
  });
});

