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
import {ModuleLoaderNodeJSPath} from "../xml/ModuleLoaderNodeJSPath";
import {ModuleLoader} from "../ModuleLoader";
import type {SerializerErrorReporter} from "../SerializerError";
import {SerializerErrorReporterStream} from "../SerializerError";

//const response = await fetch("org.ensor.nodejs.openscad.xml");
//console.log(response);
//var xmlString: string = await response.text();
//console.log("Fetched HTTP data");
//console.log(xmlString);
//const parser = new DOMParser();
//const doc1 = parser.parseFromString(xmlString, "application/xml");
//console.log(doc1.contentType); // "application/xml"
//console.log(doc1);

const err: SerializerErrorReporter = new SerializerErrorReporterStream();
const loaderPlugin: ModuleLoaderNodeJSPath = new ModuleLoaderNodeJSPath();
const loader: ModuleLoader = new ModuleLoader();
loader.setPlugin(loaderPlugin);
loaderPlugin.addPath("");
await loader.loadModule("org.ensor.nodejs.openscad", err);


import { defineNode, NodeInterface } from "@baklavajs/core";
import { BaklavaEditor, useBaklava, NumberInterface, SelectInterface } from "@baklavajs/renderer-vue";
import "@baklavajs/themes/dist/syrup-dark.css";

console.log("Defining some nodes");

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

const baklava = useBaklava();
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