<template>
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
</template>

<script setup lang="ts">
import VTabs from './VTabs.vue'
import VTab from './VTab.vue'
import {EventBus} from "./EventBus";
import {SerializerXML} from "../xml/SerializerXML";
import type {SerializerErrorReporter} from "../SerializerError";
import {SerializerErrorReporterStream} from "../SerializerError";

interface ScriptData {
    mNodeModule: NodeModule;
};

var data:ScriptData = {mNodeModule: null};

function setModule(module: NodeModule) {
	    data.mNodeModule = module;
            console.log("Set the module");
	    console.log(data.mNodeModule);
}

function doAddNodeOnClick() {
            //const node2 = new MathNode();
            //node2.position = { x: 450, y: 100 };
            //baklava.editor.graph.addNode(node2);
            //EventBus.emit("change_n", "xxx");
            console.log("Doing click");
            if (data.mNodeModule != null) {
                console.log("Loaded node module for real");
                var ser: Serializer = SerializerXML.instance();
                var outstream: WritableStream = new WritableStream({
                    write(chunk) {
                        EventBus.get().emit("changeText", { msg: chunk});
                        console.log(chunk);
                    }
		});
               const err: SerializerErrorReporter = new SerializerErrorReporterStream();
               ser.write(data.mNodeModule!, outstream, err);
            }
}

defineExpose({setModule});

</script>
