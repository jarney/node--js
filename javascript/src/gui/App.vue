<template>
  <h1>node--js node editor</h1>
      <h2 class="text-xl font-bold mb-8" id="tabs">Tabs</h2>
  <Suspense>
      <VTabs>
        <VTab title="Tab 1">
          <editor/>
	</VTab>
        <VTab title="Tab 2">Content 2</VTab>
        <VTab title="Tab 3">Content 3</VTab>
      </VTabs>
  </Suspense>
  <ul>Powered by:
  <li><a href="https://vuejs.org/" target="_blank" rel="noopener">vuejs.org</a>: General UI framework</li>
  <li><a href="https://baklavajs.org/" target="_blank" rel="noopener">baklavajs</a>: Node editor visuals</li>
  </ul>
</template>

<style scoped></style>

<script setup lang="ts">
import editor from "./editor.vue";

import VTabs from './VTabs.vue'
import VTab from './VTab.vue'

</script>


