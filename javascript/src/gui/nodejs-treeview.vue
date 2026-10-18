<template>
<div style="overflow:scroll;">
<ul>
	        <li>Module Info</li>
		<li>Imports</li>
		    <ul ref="imported-modules-container">
		    </ul>
                <li>Graphs</li>
		    <ul ref="graphs-container">
		    </ul>
		<li>Data Types</li>
		    <ul ref="data-types-container">
		    </ul>
                <li>Node Types</li>
                  <ul ref="node-types-container">
                  </ul>
             </ul>
</div>
</template>

<script setup lang="ts">
import { useTemplateRef, onMounted } from 'vue';

import {NodeModule} from "../NodeModule";
import {EventBus} from "./EventBus.ts";

interface ScriptData {
    mNodeModule: NodeModule | null;
};

var data:ScriptData = {mNodeModule: null};

var dataTypesContainer = useTemplateRef("data-types-container");
var nodeTypesContainer = useTemplateRef("node-types-container");
var graphsContainer = useTemplateRef("graphs-container");
var importedModulesContainer = useTemplateRef("imported-modules-container");

function setModule(module: NodeModule) {
    data.mNodeModule = module;

    for (var graphId in data.mNodeModule.getGraphs()) {
        var graphEl: Element = document.createElement("div");
	graphEl.setAttribute("id", graphId);
        graphEl.textContent = graphId;
        graphsContainer.value.appendChild(graphEl);
    }

    for (var importedModuleId of data.mNodeModule.getImportModuleList()) {
        var importedModuleEl: Element = document.createElement("div");
	importedModuleEl.setAttribute("id", importedModuleId);
        importedModuleEl.textContent = importedModuleId;
        importedModulesContainer.value.appendChild(importedModuleEl);
	importedModuleEl.addEventListener("click", () => {
            EventBus.get().emit("module-load", { msg: importedModuleId});
	});
    }

    for (var dataTypeLocalId in data.mNodeModule.getDataTypes()) {
        const dataType = data.mNodeModule.getDataType(dataTypeLocalId);
        var dataTypeEl: Element = document.createElement("div");
	dataTypeEl.setAttribute("id", dataType.getFullyQualifiedId());
        dataTypeEl.textContent = dataType.getName();
        dataTypesContainer.value.appendChild(dataTypeEl);
    }

    for (var nodeTypeLocalId in data.mNodeModule.getNodeTypes()) {
        const nodeType = data.mNodeModule.getNodeType(nodeTypeLocalId);
        var nodeTypeEl: Element = document.createElement("div");
	nodeTypeEl.setAttribute("id", nodeType.getFullyQualifiedId());
        nodeTypeEl.textContent = nodeType.getLocalId();
        nodeTypesContainer.value.appendChild(nodeTypeEl);
    }
}

defineExpose({setModule});

</script>
