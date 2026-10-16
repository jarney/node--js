import { describe, it, expect } from 'vitest'

import { DataType } from "../src/DataType";
import { NodeType } from "../src/NodeType";
import { NodePort } from "../src/NodePort";
import { NodeModule } from "../src/NodeModule"
import { ModuleLoader } from "../src/ModuleLoader";

describe("NodeModule", () => {
  it("test_NodeModule_package_id", function() {
    var loader: ModuleLoader  = new ModuleLoader();
    var nodeModule: NodeModule = loader.newModule("anonymous");
    
    nodeModule.setPackage("simple.package.name");
    nodeModule.setDescription("Some Desc");

    expect.assert(nodeModule.getPackage() == "simple.package.name");
    expect.assert(nodeModule.getDescription() == "Some Desc");
  });

  it("test_NodeModule_single_type", function() {
    var loader: ModuleLoader  = new ModuleLoader();
    var nodeModule: NodeModule = loader.newModule("anonymous");
    
    var nodeType: NodeType = new NodeType();
    nodeType.setId("add");
    nodeType.getInputs().addPort("first", new NodePort("variable", "First Argument"));
    
    nodeModule.addNodeType(nodeType);

    expect.assert(nodeModule.hasNodeType("add"));
    
    var nt: NodeType | undefined = nodeModule.getNodeType("add");
    
    expect.assert(nt!.getId() == "add");
    expect.assert(nt!.getInputs().getCount() == 1);

    var nonexistent: NodeType | undefined = nodeModule.getNodeType("invalid-node");
    expect.assert(nonexistent == undefined);

    // This is actually really dangerous and maybe we should
    // not even have such a method because it would
    // orphan actual nodes in graphs.
    nodeModule.removeNodeType("add");
    expect.assert(!nodeModule.hasNodeType("add"));
  });

  it("test_NodeModule_iterator", function() {
    var loader: ModuleLoader  = new ModuleLoader();
    var nodeModule: NodeModule = loader.newModule("anonymous");
    
    var nodeType: NodeType = new NodeType();
    nodeType.setId("add");
    nodeType.getInputs().addPort("first", new NodePort("variable", "First Argument"));
    nodeModule.addNodeType(nodeType);

    var i: number = 0;
    for (var key in nodeModule.getNodeTypes()) {
	expect.assert(key == "add");
	i++;
    }
    // Check that we actually iterated.
    expect.assert(i == 1);
  });

  it("test_NodeModule_check_basic_data_type_stuff", function() {
    var loader: ModuleLoader  = new ModuleLoader();
    var nodeModule: NodeModule = loader.newModule("anonymous");

    var dataType: DataType = new DataType("variable", "pancake", "Variable Type");
    nodeModule.addDataType(dataType);

    expect.assert(nodeModule.hasDataType("variable"));
    expect.assert(nodeModule.getDataType("variable").getName() == "Variable Type");

    nodeModule.removeDataType("variable");

    expect.assert(!nodeModule.hasDataType("variable"));
    expect.assert(nodeModule.getDataType("variable") == undefined);
  });

  it("test_NodeModule_data_type_iterator", function() {
    var loader: ModuleLoader  = new ModuleLoader();
    var nodeModule: NodeModule = loader.newModule("anonymous");
    
    var dataType: DataType = new DataType("variable", "namespace", "Variable Type");
    nodeModule.addDataType(dataType);

    var i: number = 0;
    for (var key in nodeModule.getDataTypes()) {
      expect.assert(key == "variable");
      expect.assert(nodeModule.getDataType(key).getFullyQualifiedId() == "namespace.variable");
      i++;
    }
    // Check that we actually iterated.
    expect.assert(i == 1);
  });

  it("test_NodeModule_getModuleLoader", function() {
    var loader: ModuleLoader  = new ModuleLoader();
    var nodeModule: NodeModule = loader.newModule("anonymous");

    expect.assert(loader == nodeModule.getModuleLoader());
  });

  it("test_NodeModule_add_graph_uniqueness_conditions", function() {
    var loader: ModuleLoader  = new ModuleLoader();
    var nodeModule: NodeModule = loader.newModule("anonymous");

    var graph1: NodeGraph = nodeModule.addGraph("graph");
    var graph1_out: NodeGraph = nodeModule.getGraph("graph");
    expect.assert(graph1 == graph1_out);

    var graph2: NodeGraph = nodeModule.addGraph("graph");

    expect.assert(graph1 != undefined);
    expect.assert(graph2 == undefined);

    expect.assert(nodeModule.getGraph("nonexistent") == undefined);
  });
});
