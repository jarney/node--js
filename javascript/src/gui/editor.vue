<template>
  <div class="nodejs">
    <div class="nodejs-container">
      <div class="nodejs-left-sidebar">
      <VTabs>
        <VTab title="Graphs">
          <ul>
            <li>main</li>
            <li>goxhoin</li>
	</ul>
	</VTab>
        <VTab title="Palette">
	  Showing all node types available/imported.
	  Organize them by some kind of metadata tags.
	  <ul>
            <li><button @click="doAddNodeOnClick">Create Node</button></li>
            <li>sphere</li>
            <li>extrude</li>
            <li>translate</li>
            <li>union</li>
	  </ul>
	</VTab>
        <VTab title="Module">
	  <ul>
            <li>Module namespace/package</li>
	    <li>Description</li>
	    <li>Imported namespaces</li>
            <li>Module-level metadata</li>
	  </ul>
	</VTab>
        <VTab title="Node Types">
	  <ul>
            <li>sphere</li>
            <li>extrude</li>
            <li>translate</li>
            <li>union</li>
	  </ul>
	</VTab>
        <VTab title="Data Types">
	  Editing the data-types that are defined in this module.
	  <ul>
            <li>sphere</li>
            <li>extrude</li>
            <li>translate</li>
            <li>union</li>
	  </ul>
	</VTab>
      </VTabs>
      </div>
      <div class="nodejs-content">
          <div class="editor-container">
              <BaklavaEditor :view-model="baklava" />
	  </div>
      </div>
      <div class="nodejs-right-sidebar">
          When a node is selected, detailed information about the node appears here.
	  <ul>
	  <li>* Node level metadata</li>
	  <li>* Node level metadata</li>
	  </ul>
	  When a node is not selected, detailed information about the graph appears here.
	  <ul>
	  <li>* Graph level metadata</li>
	  </ul>
      </div>
    </div>
  </div>
    <div >
      <VTabs>
        <VTab title="Source">
            <div style="height: 90px; overflow-y: scroll;">
                <TextBox></TextBox>
            </div>
	</VTab>
        <VTab title="Output">
	  XML of the graph (module) goes here.
	  This is a large scroll area with a 'copy' button.
	</VTab>
      </VTabs>
    </div>
</template>

<script setup lang="ts">
import VTabs from "./VTabs.vue";
import VTab from "./VTab.vue";
import TextBox from "./TextBox.vue";
import {EventBus} from "./EventBus";

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

console.log("Defining node interface types:");

const baklava = useBaklava();
baklava.settings.enableMinimap = true;


var uiDataTypes: { [key: string] : NodeInterfaceType<string>} = {};

var populateBaklava: PopulateBaklava = new PopulateBaklava(nodeModule!, baklava);

populateBaklava.run()

console.log("About to use a data type");
console.log(uiDataTypes);
console.log(uiDataTypes["org.ensor.nodejs.openscad.variable"]);

// Next, define all of the node types:

const MathNode = defineNode({
    type: "MathNode",
    inputs: {
        number1: () => new NumberInterface("Number", 1),
        number2: () => new NumberInterface("Number", 10),
        operation: () => new SelectInterface("Operation", "Add", ["Add", "Subtract"]).setPort(false),
    },
    outputs: {
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
    //EventBus.emit("change_n", "xxx");


    if (nodeModule != undefined) {
       var ser: Serializer = SerializerXML.instance();
       var outstream: WritableStream = new WritableStream({
           write(chunk) {
                   EventBus.get().emit("changeText", { msg: chunk});
                   //console.log(chunk);
           }
       });
       ser.write(nodeModule!, outstream, err);
}    



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