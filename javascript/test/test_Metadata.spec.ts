import { describe, it, expect } from "vitest";

import { Metadata } from "../src/Metadata.ts";

describe("Metadata", function(){
    it("test_Metadata_equality", function() {
        var metadata = new Metadata();
    
        expect.assert(!metadata.hasMetadata("some-entry"));
        
	// Retrieving a non-existent entry should create it.
        var cd = metadata.getMetadata("some-entry")
        cd.setValue("a", "b");
        expect.assert(cd.getValue("a") == "b")


        // Check that when we get it again, it already exists and has our data.
        expect.assert(metadata.hasMetadata("some-entry"));
        var cd2 = metadata.getMetadata("some-entry")
        expect.assert(cd2.getValue("a") == "b")

	var ns = metadata.getNamespaces();
	expect(ns).toEqual(["some-entry"]);
    });
});