<template>
  <div class="nodejs">
    <div class="nodejs-container">
      <div class="nodejs-left-sidebar">
      <ul>
          <li>Graph Selection</li>
          <li>Palette</li>
	  <li>Type Editor</li>
      </ul>
          <p>Select Node to place</p>
          <ul id="node-tree">
              <li><button @click="doAddNodeOnClick">Create Node</button></li>
	  </ul>
      </div>
      <div class="nodejs-content">
          <div class="editor-container">
              <BaklavaEditor :view-model="baklava" />
	  </div>
      </div>
      <div class="nodejs-right-sidebar">
          Node-specific editing can happen here based on what node has been
	  selected.
      </div>
    </div>
  </div>
    <div >
        Output goes here in scroll area along bottom?
    </div>
</template>

<script setup lang="ts">
/*
 * Inventory of things for the module editor:
 *    * Place to extract the model xml: copy button?
 *    * Palette for listing the 'tree' of available nodes.
 *    * Place to select/open different graphs in the model.
 *    * Way to 'import/select' other modules (i.e. openscad, elisp, c, llvm, etc)
 *    * Place to 'render' the output of the model's 'Processor'
 *          Probably a window along the bottom?
 */
// node--js stuff
import {ModuleLoaderHTTP} from "../xml/ModuleLoaderHTTP";
import {ModuleLoader} from "../ModuleLoader";
import {NodeModule} from "../NodeModule";
import type {Serializer} from "../Serializer";
import type {SerializerErrorReporter} from "../SerializerError";
import {SerializerErrorReporterStream} from "../SerializerError";
import {SerializerXML} from "../xml/SerializerXML";
import {DataType} from "../DataType";

// baklavajs stuff
import { defineNode, NodeInterface } from "@baklavajs/core";
import { BaklavaEditor, useBaklava, NumberInterface, SelectInterface } from "@baklavajs/renderer-vue";
import "@baklavajs/themes/dist/syrup-dark.css";
import { NodeInterfaceType } from "@baklavajs/interface-types";

// bridge between node--js and baklavajs
import { PopulateBaklava } from "./PopulateBaklava";

const err: SerializerErrorReporter = new SerializerErrorReporterStream();
const loaderPlugin: ModuleLoaderHTTP = new ModuleLoaderHTTP();
const loader: ModuleLoader = new ModuleLoader();
loader.setPlugin(loaderPlugin);
loaderPlugin.addPath("");

const nodeModule: NodeModule | undefined = await loader.loadModule("foo", err);

if (nodeModule != undefined) {
   var ser: Serializer = SerializerXML.instance();
   var outstream: WritableStream = new WritableStream({
       write(chunk) {
               console.log(chunk);
       }
   });
//   ser.write(nodeModule!, outstream, err);
}    


console.log("Defining node interface types:");

const baklava = useBaklava();
baklava.settings.enableMinimap = true;


var uiDataTypes: { [key: string] : NodeInterfaceType<string>} = {};

var populateBaklava: PopulateBaklava = new PopulateBaklava(nodeModule!, baklava);

populateBaklava.run()

import { setType } from "@baklavajs/interface-types";

console.log("About to use a data type");
console.log(uiDataTypes);
console.log(uiDataTypes["org.ensor.nodejs.openscad.variable"]);

// Next, define all of the node types:

const MathNode = defineNode({
    type: "MathNode",
    inputs: {
//        "foo": () => new NodeInterface("Radius", "default").use(setType, uiDataTypes["org.ensor.nodejs.openscad.variable"]!),
        number1: () => new NumberInterface("Number", 1),
        number2: () => new NumberInterface("Number", 10),
        operation: () => new SelectInterface("Operation", "Add", ["Add", "Subtract"]).setPort(false),
    },
    outputs: {
//        "out": () => new NodeInterface("FnDef", "default").use(setType, uiDataTypes["org.ensor.nodejs.openscad.function-definition"]!),
        output: () => new NodeInterface("Output", 0),
    },
});

baklava.editor.registerNodeType(MathNode);
baklava.settings.palette.enabled = false;
baklava.settings.toolbar.enabled = false;

const node1 = new MathNode();
node1.position = { x: 100, y: 100 };
baklava.editor.graph.addNode(node1);

const node2 = new MathNode();
node2.position = { x: 450, y: 100 };
baklava.editor.graph.addNode(node2);

function doAddNodeOnClick() {
    const node2 = new MathNode();
    node2.position = { x: 450, y: 100 };
    baklava.editor.graph.addNode(node2);
}

console.log("Adding a connection in our graph");

baklava.editor.graph.addConnection(node1.outputs.output, node2.inputs.number1);

console.log("Initialization complete, graph visible?");

function doit() {
  console.log("asdfasdfadfadfasdf");
}

</script>

<style scoped>
.settings-example {
    display: flex;
    flex-direction: column;
    height: 100%;
}

.settings-controls {
    display: flex;
    gap: 16px;
    flex-wrap: wrap;
    padding: 12px 16px;
    background: var(--vp-c-bg-soft);
    border-bottom: 1px solid var(--vp-c-divider);
    font-size: 14px;
}

.settings-controls label {
    display: flex;
    align-items: center;
    gap: 6px;
    cursor: pointer;
}

.editor-container {
    height: 500px;
/*    position: relative;*/
/*    flex: 1;*/
}

.nodejs {
  display: table;
  width: 100%;
}

.nodejs-container {
    display: table-row;
}
.nodejs-left-sidebar {
    display: table-cell;
    width: 15%;
    vertical-align: top;
}
.nodejs-content {
    display: table-cell;
    width: 70%;
}
.nodejs-right-sidebar {
    display: table-cell;
    width: 15%;
    vertical-align: top;
}

</style>