import { describe, it, expect } from 'vitest'

import { NodeType } from "../src/NodeType.ts";
import { NodePort } from "../src/NodePort.ts";
import { ConnectionData } from "../src/ConnectionData.ts";
import { Metadata } from "../src/Metadata.ts";

describe("NodeType", () => {
  it("test_NodeType_empty", () => {

    var nodeType = new NodeType();
    
    nodeType.setId("add");
    nodeType.setVisibility(NodeType.Visibility.PRIVATE);
    nodeType.setType(NodeType.Type.NATIVE);

    expect(nodeType.getVisibility()).to.equal(NodeType.Visibility.PRIVATE);
    expect(nodeType.getType()).to.equal(NodeType.Type.NATIVE);

    nodeType.setVisibility(NodeType.Visibility.PUBLIC);
    nodeType.setType(NodeType.Type.GRAPH);

    expect(nodeType.getVisibility()).to.equal(NodeType.Visibility.PUBLIC);
    expect(nodeType.getType()).to.equal(NodeType.Type.GRAPH);
  });

it("test_NodeType_some_ports", () => {
    var nodeType = new NodeType();
    nodeType.setId("add");
    nodeType.getInputs().addPort("first", new NodePort("variable", "First Argument"));
    nodeType.getInputs().addPort("second", new NodePort("variable", "Second Argument"));
    nodeType.getOutputs().addPort("output", new NodePort("variable", "Result"));

    expect.assert(nodeType.getInputs().getCount() == 2);
    expect.assert(nodeType.getOutputs().getCount() == 1);

    expect.assert(nodeType.getInputs().getByName("first").getDescription() == "First Argument");
    expect.assert(nodeType.getInputs().getName(0) == "first");
    expect.assert(nodeType.getInputs().getByIndex(0).getDescription() == "First Argument");
    expect.assert(nodeType.getInputs().hasPort("first"));
    
    expect.assert(nodeType.getInputs().getByName("second").getDescription() == "Second Argument");
    expect.assert(nodeType.getInputs().getName(1) == "second");
    expect.assert(nodeType.getInputs().getByIndex(1).getDescription() == "Second Argument");

    expect.assert(nodeType.getOutputs().getByName("output").getDescription() == "Result");
    expect.assert(nodeType.getOutputs().getName(0) == "output");
    expect.assert(nodeType.getOutputs().getByIndex(0).getDescription() == "Result");
    expect.assert(nodeType.getOutputs().hasPort("output"));

    // Now, some edge cases:
    expect.assert(nodeType.getInputs().getByName("non-existent") == undefined);
    expect.assert(nodeType.getOutputs().getByName("non-existent") == undefined);
    expect.assert(!nodeType.getInputs().hasPort("non-existent"));
    expect.assert(!nodeType.getOutputs().hasPort("non-existent"));
    expect.assert(nodeType.getInputs().getName(99) == "");
    expect.assert(nodeType.getOutputs().getName(99) == "");
    expect.assert(nodeType.getInputs().getByIndex(99) == undefined);
    expect.assert(nodeType.getOutputs().getByIndex(1) == undefined);
});

it("test_NodeType_unique_input_ports", () => {
    var nodeType = new NodeType();
    var rc1: boolean  = nodeType.getInputs().addPort("first", new NodePort("variable", "First Argument"));
    var rc2: boolean = nodeType.getInputs().addPort("first", new NodePort("variable", "Another"));

    expect.assert(rc1 == true);
    expect.assert(rc2 == false);

    expect.assert(nodeType.getInputs().getCount() == 1);
    expect.assert(nodeType.getOutputs().getCount() == 0);
    expect.assert(nodeType.getInputs().getByName("first").getDescription() == "First Argument");
});

it("test_NodeType_unique_output_ports", () => {
    var nodeType = new NodeType();
    var rc1: boolean = nodeType.getOutputs().addPort("first", new NodePort("variable", "First Argument"));
    var rc2: boolean = nodeType.getOutputs().addPort("first", new NodePort("variable", "Another"));

    expect.assert(rc1 == true);
    expect.assert(rc2 == false);

    expect.assert(nodeType.getInputs().getCount() == 0);
    expect.assert(nodeType.getOutputs().getCount() == 1);
    expect.assert(nodeType.getOutputs().getByName("first").getDescription() == "First Argument");
});

it("test_NodeType_default_node_data", () => {
    var nodeType = new NodeType();

    var defaultData = new ConnectionData();
    defaultData.setValue("x", "foo");
    nodeType.setDefaultNodeData(defaultData);

    expect.assert(nodeType.getDefaultNodeData().getValue("x") == "foo");
    
});

});
