<template>
    <div class="nodejs">
        <div class="nodejs-container">
            <jse-split-vertical>
                <div class="nodejs-left-sidebar">
		  <jse-left-sidebar ref="leftSidebar"/>
                </div>
                <div class="nodejs-content">
                    <div class="editor-container">
                        <BaklavaEditor :view-model="baklava" />
                    </div>
                </div>
                <div class="nodejs-right-sidebar">
                    <jse-right-sidebar/>
                </div>
            </jse-split-vertical>
            <jse-bottom/>
        </div>
    </div>
</template>

<script setup lang="ts">
import VTabs from "./VTabs.vue";
import VTab from "./VTab.vue";
import TextBox from "./TextBox.vue";
import {EventBus} from "./EventBus";
import jseSplitVertical from "./jseSplitVertical.vue";
import jseLeftSidebar from "./leftSidebar.vue";
import jseRightSidebar from "./rightSidebar.vue";
import jseBottom from "./jseBottom.vue";

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

import type {IBaklavaViewModel} from "@baklavajs/renderer-vue";
import { useTemplateRef, onMounted } from 'vue';

var baklava = useBaklava();
baklava = useBaklava();
baklava.settings.enableMinimap = true;
baklava.settings.palette.enabled = false;
baklava.settings.toolbar.enabled = false;

const leftSidebar = useTemplateRef('leftSidebar')!;

function onModuleLoaded(nodeModule: NodeModule) {
    console.log("Got template ref");
    console.log(leftSidebar);
    leftSidebar.value.setModule(nodeModule);
    console.log("Defining node interface types:");
            
    var populateBaklava: PopulateBaklava = new PopulateBaklava(nodeModule, baklava);
    populateBaklava.run()
            
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
    	}
    });

    baklava.editor.registerNodeType(MathNode);

    const node1 = new MathNode();
    node1.position = { x: 100, y: 100 };
    baklava.editor.graph.addNode(node1);
    
    const node2 = new MathNode();
    node2.position = { x: 450, y: 100 };
    baklava.editor.graph.addNode(node2);
            
    console.log("Adding a connection in our graph");
    
    baklava.editor.graph.addConnection(node1.outputs.output, node2.inputs.number1);
    
    console.log("Initialization complete, graph visible?");
}

onMounted(() => {
    console.log("Mounted");
        const err: SerializerErrorReporter = new SerializerErrorReporterStream();
        const loaderPlugin: ModuleLoaderHTTP = new ModuleLoaderHTTP();
        const loader: ModuleLoader = new ModuleLoader();
        loader.setPlugin(loaderPlugin);
        loaderPlugin.addPath("");

	loader
	    .loadModule("foo", err)
	    .then((nodeModule: NodeModule | undefined) => {onModuleLoaded(nodeModule!);});

});

</script>

<style scoped>
.editor-container {
    height: 500px;
/*    position: relative;*/
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