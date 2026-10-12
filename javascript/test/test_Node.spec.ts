import { describe, it, expect } from 'vitest'

import { NodeGraph } from "../src/NodeType.ts";
import { NodeModule } from "../src/NodeType.ts";
import { NodeType } from "../src/NodeType.ts";
import { Node } from "../src/Node.ts";
import { NodePort } from "../src/NodePort.ts";
import { ConnectionData } from "../src/ConnectionData.ts";
import { Metadata } from "../src/Metadata.ts";
import { ModuleLoaderNodeJSPath } from "../src/xml/ModuleLoaderNodeJSPath.ts";

const DATA_TYPE: string = "variable";

describe("Node", () => {
  it("test_Node_create_node", function() {
    var loader: ModuleLoaderNodeJSPath = new ModuleLoaderNodeJSPath();
    var module: NodeModule = loader.newModule("anonymous");
    var graph: NodeGraph = module.addGraph("graph");;
    
    var nodeType: NodeType = new NodeType();
    
    // First, just check identity stuff.
    var createdNode: Node = graph.newNode(nodeType, "main");
    expect.assert(createdNode.getId() == "main");
    expect.assert(createdNode.getType() == nodeType);
  });
  
  it("test_Node_type_io", function() {
    var loader: ModuleLoaderNodeJSPath = new ModuleLoaderNodeJSPath();
    var module: NodeModule = loader.newModule("anonymous");
    var graph: NodeGraph = module.addGraph("graph");
    
    var nodeType: NodeType = new NodeType();
    nodeType.getInputs().addPort("a", new NodePort(DATA_TYPE, "adesc", NodePort.ConnectionPolicy.Multiple));
    nodeType.getInputs().addPort("b", new NodePort(DATA_TYPE, "bdesc", NodePort.ConnectionPolicy.Multiple));
    nodeType.getOutputs().addPort("out", new NodePort(DATA_TYPE, "out", NodePort.ConnectionPolicy.Multiple));
    
    var node: Node = graph.newNode(nodeType, "main");
    
    expect.assert(node.getType().getInputs().getCount() == 2);
    expect.assert(node.getType().getOutputs().getCount() == 1);
    
    // So we can validate that the node type's data is correct,
    // but we should also verify the node-specific port info.
    expect.assert(node.getType().getInputs().getByName("a").getDescription() == "adesc");
    expect.assert(node.getType().getInputs().getByName("b").getDescription() == "bdesc");
    expect.assert(node.getType().getOutputs().getByName("out").getDescription() == "out");
    
    expect.assert(node.getInputs().getByName("a").getDescription() == "adesc");
    expect.assert(node.getInputs().getByName("b").getDescription() == "bdesc");
    expect.assert(node.getOutputs().getByName("out").getDescription() == "out");
  });
  
  it("test_Node_specific_io", function() {
    var loader: ModuleLoaderNodeJSPath = new ModuleLoaderNodeJSPath();
    var module: NodeModule = loader.newModule("anonymous");
    var graph: NodeGraph = module.addGraph("graph");
    
    var nodeType: NodeType = new NodeType();
    
    var node: Node = graph.newNode(nodeType, "main");
    
    node.getOverrideInputs().addPort("a", new NodePort(DATA_TYPE, "adesc", NodePort.ConnectionPolicy.Multiple));
    node.getOverrideInputs().addPort("b", new NodePort(DATA_TYPE, "bdesc", NodePort.ConnectionPolicy.Multiple));
    node.getOverrideOutputs().addPort("out", new NodePort(DATA_TYPE, "out", NodePort.ConnectionPolicy.Multiple));
    
    expect.assert(!node.hasOverrideInputs());
    expect.assert(!node.hasOverrideOutputs());
    
    // Check that we've set the override ports
    expect.assert(node.getOverrideInputs().getCount() == 2);
    expect.assert(node.getOverrideOutputs().getCount() == 1);
    
    // Check that they have not yet taken effect.
    expect.assert(node.getInputs().getCount() == 0);
    expect.assert(node.getOutputs().getCount() == 0);
    
    // When we set the node to override, the port counts
    // should change based on what we set up.
    node.setOverrideInputs(true);
    node.setOverrideOutputs(true);
    
    expect.assert(node.hasOverrideInputs());
    expect.assert(node.hasOverrideOutputs());
    expect.assert(node.getInputs().getCount() == 2);
    expect.assert(node.getOutputs().getCount() == 1);
  });
  
  it("test_Node_initialize_with_data", function() {
    var loader: ModuleLoaderNodeJSPath = new ModuleLoaderNodeJSPath();
    var module: NodeModule = loader.newModule("anonymous");
    var graph: NodeGraph = module.addGraph("graph");
    
    var nodeType: NodeType = new NodeType();
    var initialData: ConnectionData = new ConnectionData();
    initialData.setValue("x", "some-initial-value");
    
    var node: Node = graph.newNode(nodeType, "main", initialData);
    
    // Verify that we initialized the data
    expect.assert(node.getData().getValue("x") == "some-initial-value");
    
    // Verify that if we change the initial data, the node doesn't change
    initialData.setValue("x", "changed our default");
    
    expect.assert(initialData.getValue("x") == "changed our default");
    expect.assert(node.getData().getValue("x") == "some-initial-value");
    
    // Verify that if we change the node's data, it
    // is different than the original copy.
    node.getData().setValue("x", "changed node-specific data");
    
    expect.assert(initialData.getValue("x") == "changed our default");
    expect.assert(node.getData().getValue("x") == "changed node-specific data");
    
  });
  
  it("test_Node_specific_metadata", function() {
    var loader: ModuleLoaderNodeJSPath = new ModuleLoaderNodeJSPath();
    var module: NodeModule = loader.newModule("anonymous");
    var graph: NodeGraph = module.addGraph("graph");
    
    var nodeType: NodeType = new NodeType();
    
    var node: Node = graph.newNode(nodeType, "main");
    
    var metadata: Metadata = node.getMetadata();
    
    // Check that we don't have any specific metadata yet.
    expect.assert(!metadata.hasMetadata("foo.bar.org"));
    
    // Check that when we ask for metadata, it's created
    var cd: ConnectionData = metadata.getMetadata("foo.bar.org");
    expect.assert(metadata.hasMetadata("foo.bar.org"));
    
    cd.setValue("x", "some-value");
    
    // Javascript/TypeScript has no such thing as const-ness, so
    // we don't worry about the rest of this test
    // Check that we can make a const-version of this.
    //const Node & constNode = node;
    //const Metadata & constMetadata = constNode.getMetadata();
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
