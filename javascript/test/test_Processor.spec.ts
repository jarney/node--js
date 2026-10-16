import { describe, it, expect } from "vitest";

import type { NodeProcessor } from "../src/Processor";

import {Node} from "../src/Node";
import {NodeType} from "../src/NodeType";
import {NodeModule} from "../src/NodeModule";
import {ConnectionData} from "../src/ConnectionData";
import {NodeGraph} from "./src/NodeGraph";
import {Processor} from "../src/Processor";
import {ModuleLoader} from "../src/ModuleLoader";

class NodeProcessor_binary implements NodeProcessor {
  public process(
    processor: Processor,
    node: Node,
    fromData: ConnectionData,
    toData: ConnectionData
  ) : void {
    toData.setValue("out", "binary call node(" + fromData.getValue("in1") + ", " + fromData.getValue("in2") + ")");
  }
};

class NodeProcessor_unary implements NodeProcessor {
  public process(
    processor: Processor,
    node: Node,
    fromData: ConnectionData,
    toData: ConnectionData
  ) : void {
    toData.setValue("out", "callDataFrom(" + node.getId() + ")");
  }
};

describe("Processor", function(){
  it("test_Processor_minimal", function() {
    var loader: ModuleLoader = new ModuleLoader();
    var module: NodeModule = loader.newModule("anonymous");
    var graph: NodeGraph = module.addGraph("graph");;

    var unaryType: NodeType = new NodeType();
    unaryType.setId("unary", "scad");
    unaryType.setType(NodeType.Type.NATIVE);

    var binType: NodeType = new NodeType();
    binType.setId("binary", "scad");
    binType.setType(NodeType.Type.NATIVE);
    
    var main: Node = graph.newNode(binType, "main")!;
    var call1: Node = graph.newNode(unaryType, "call1")!;
    var call2: Node = graph.newNode(unaryType, "call2")!;

    graph.newEdge(call1.getId(), "out", main.getId(), "in1");
    graph.newEdge(call2.getId(), "out", main.getId(), "in2");

    var processor: Processor = new Processor();
    processor.setNativeImpl("scad.unary", new NodeProcessor_unary());
    processor.setNativeImpl("scad.binary", new NodeProcessor_binary());
    
    var input: ConnectionData = new ConnectionData();
    var output: ConnectionData = new ConnectionData();
    processor.processGraph(graph, input, output);

    console.log("Out is ");
    console.log(output.getValue("out"));
    console.log("binary call node(callDataFrom(call1), callDataFrom(call2))");
    expect.assert(output.getValue("out") == "binary call node(callDataFrom(call1), callDataFrom(call2))");
  });
});
