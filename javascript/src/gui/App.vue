<template>
<div style="display: flex; flex-direction: column; height: 100vh;">
    <div style="flex: 1;">
        <h1 class="text-xl font-bold mb-8" id="tabs">node--js Editor</h1>
        <editor/>
    </div>
    <div style="">
        <ul>Powered by:
            <li><a href="https://vuejs.org/" target="_blank" rel="noopener">vuejs.org</a>: General UI framework</li>
            <li><a href="https://baklavajs.org/" target="_blank" rel="noopener">baklavajs</a>: Node editor visuals</li>
        </ul>
    </div>
</div>
</template>

<style scoped></style>

<script setup lang="ts">
import editor from "./editor.vue";

import VTabs from './VTabs.vue'
import VTab from './VTab.vue'
import jseSplitVertical from "./jseSplitVertical.vue"

</script>


<style scoped>
</style>
