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
import {ModuleLoaderHTTP} from "../xml/ModuleLoaderHTTP";
import {ModuleLoader} from "../ModuleLoader";
import {NodeModule} from "../NodeModule";
import type {Serializer} from "../Serializer";
import type {SerializerErrorReporter} from "../SerializerError";
import {SerializerErrorReporterStream} from "../SerializerError";
import {SerializerXML} from "../xml/SerializerXML";


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

import { defineNode, NodeInterface } from "@baklavajs/core";
import { BaklavaEditor, useBaklava, NumberInterface, SelectInterface } from "@baklavajs/renderer-vue";
import "@baklavajs/themes/dist/syrup-dark.css";

console.log("Defining some nodes");

// Define all of the data types first.
var nodeInterfaceTypes = {};
for (const ns of loader.getLoadedModules()) {
    var mod: NodeModule = loader.getLoadedModule(ns)!;
    console.log("Loaded module " + ns);
    var dataTypeMap: { [ key: string ] : DataType } = mod.getDataTypes();
    for (const id in dataTypeMap) {
        console.log("Defining type " + id);
        var dataType: DataType = dataTypeMap[id]!;
	var fn = function() {
	    return new NodeInterface(dataType.getName());
	};
	nodeInterfaceTypes[dataType.getFullyQualifiedId()] = fn;
    }
}
console.log(nodeInterfaceTypes);

const bni = new NodeInterface("myType");

const baklava = useBaklava();

var uiNodeType = {};
for (const ns of loader.getLoadedModules()) {
    var mod: NodeModule = loader.getLoadedModule(ns)!;
    console.log("Loaded module " + ns);
    var nodeTypeMap: { [ key: string ] : NodeType } = mod.getNodeTypes();
    console.log(nodeTypeMap);
    for (const id in nodeTypeMap) {
        console.log("Defining type " + id);
        var nodeType: NodeType = nodeTypeMap[id]!;
	var inputMap = {};
	var inputs: NamedPorts = nodeType.getInputs();
	
        for (var i: number = 0; i < inputs.getCount(); i++) {
            const id: string = inputs.getName(i);
	    const port: NodePort = inputs.getByIndex(i)!;
    	    console.log("    Port " + port.getDataType());
	    var dt: string = port.getDataType();
	    inputMap[id] = () => nodeInterfaceTypes[dt]();
        }
	
	uiNodeType[nodeType.getFullyQualifiedId()] = defineNode({
	type: nodeType.getLocalId(),
	inputs: inputMap,
	outputs: {
	    "d": () => nodeInterfaceTypes["org.ensor.nodejs.openscad.variable"](),
	}
	});
        console.log("Registering node type " + nodeType.getFullyQualifiedId());
        baklava.editor.registerNodeType(uiNodeType[nodeType.getFullyQualifiedId()]);
    }
}


// Next, define all of the node types:

const MathNode = defineNode({
    type: "MathNode",
    inputs: {
    "two": () => nodeInterfaceTypes["org.ensor.nodejs.openscad.variable"](),
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