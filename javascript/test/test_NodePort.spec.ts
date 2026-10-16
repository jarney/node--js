import { describe, it, expect } from "vitest";

import { NodePort } from "../src/NodePort";

describe("NodePort", function(){

    it("test_NodePort_multiple", function(){
        var DATA_TYPE = "variable";
	var DESCRIPTION = "Input A";
	var nodePort = new NodePort(DATA_TYPE, DESCRIPTION, NodePort.ConnectionPolicy.Multiple);

	expect.assert(nodePort.getDataType() == DATA_TYPE);
	expect.assert(nodePort.getDescription() == DESCRIPTION);
	expect.assert(nodePort.getConnectionPolicy() == NodePort.ConnectionPolicy.Multiple);
    });
    
    it("test_NodePort_default", function() {
	var DATA_TYPE = "variable";
	var DESCRIPTION = "Input A";
	var nodePort = new NodePort(DATA_TYPE, DESCRIPTION);
	
	expect.assert(nodePort.getDataType() == DATA_TYPE);
	expect.assert(nodePort.getDescription() == DESCRIPTION);
	expect.assert(nodePort.getConnectionPolicy() == NodePort.ConnectionPolicy.One);
    });

    it("test_NodePort_explicit_one", function() {
	var DATA_TYPE = "variable";
	var DESCRIPTION = "Input A";
	var nodePort = new NodePort(DATA_TYPE, DESCRIPTION, NodePort.ConnectionPolicy.One);
	
	expect.assert(nodePort.getDataType() == DATA_TYPE);
	expect.assert(nodePort.getDescription() == DESCRIPTION);
	expect.assert(nodePort.getConnectionPolicy() == NodePort.ConnectionPolicy.One);
	
      });

    it("test_NodePort_specific_metadata", function() {
	var DATA_TYPE = "variable";
	var DESCRIPTION = "Input A";
	var nodePort = new NodePort(DATA_TYPE, DESCRIPTION);
	
	var metadata = nodePort.getMetadata();
	
	// Check that we don't have any specific metadata yet.
	expect.assert(!metadata.hasMetadata("foo.bar.org"));
	
	// Check that when we ask for metadata, it's created
	var cd = metadata.getMetadata("foo.bar.org");
	expect.assert(metadata.hasMetadata("foo.bar.org"));
	
	cd.setValue("x", "some-value");
	
        // Javascript/TypeScript has no such thing as const-ness, so
        // we don't worry about the rest of this test
	// Check that we can make a const-version of this.
	//const NodePort & constNodePort = nodePort;
	//const Metadata & constMetadata = constNodePort.getMetadata();
	//const ConnectionData & constData = constMetadata.getMetadata("foo.bar.org");
       	//expect.assert(constData.hasValue("x"));
	//
	//const ConnectionData & readonly = constMetadata.getMetadata("some-other-namespace");
	//// Check that in the const context, we didn't actually add the namespace,
	//// just faked it by returning an empty connection data.
	//expect.assert(!constMetadata.hasMetadata("some-other-namespace"));
	//expect.assert(!readonly.hasValue("x"));
      });
  });
