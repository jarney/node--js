<template>
    <div class="settings-example">
        <div class="settings-controls">
            <label><input type="checkbox" v-model="baklava.settings.enableMinimap" /> Minimap</label>
            <label
                ><input type="checkbox" v-model="baklava.settings.useStraightConnections" /> Straight connections</label
            >
            <label><input type="checkbox" v-model="baklava.settings.displayValueOnHover" /> Show value on hover</label>
            <label><input type="checkbox" v-model="baklava.settings.nodes.resizable" /> Resizable nodes</label>
        </div>
        <div class="editor-container">
            <h1>s</h1>
            <BaklavaEditor :view-model="baklava" />
            <h1>e</h1>
        </div>
    </div>
</template>

<script setup lang="ts">

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

console.log("Adding a connection in our graph");

baklava.editor.graph.addConnection(node1.outputs.output, node2.inputs.number1);

console.log("Initialization complete, graph visible?");

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

</style>