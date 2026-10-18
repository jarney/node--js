<template>
<div style="display: flex; flex-direction: column; height: 100%;">
    <div style="flex: 1;">
        <h1 class="text-xl font-bold mb-8" id="tabs">node--js Editor</h1>
        <editor/>
    </div>
    <div style="flex: 0;">
<p>Powered by: <a href="https://vuejs.org/" target="_blank" rel="noopener">vuejs.org</a> and <a href="https://baklavajs.org/" target="_blank" rel="noopener">baklavajs</a></p>
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
