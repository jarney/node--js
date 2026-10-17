<template>
  <h1 class="text-xl font-bold mb-8" id="tabs">node--js Editor</h1>
  <jse-split-vertical>
      <div class="split__left">content-1</div>
      <div class="split__bar"></div>
      <div class="split__right">content-2</div>
      <div class="split__bar"></div>
      <div class="split__right">content-3</div>
  </jse-split-vertical>

<!--
  <Suspense>
          <editor/>
  </Suspense>
-->
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
import jseSplitVertical from "./jseSplitVertical.vue"

</script>


<style scoped>
.split__left {
  background-color: #404040;
}

.split__right {
  background-color: #C5C6B6;
}

.split__bar {
  background-color: #3C3530;
  width: 15px;
  height: 100%;
  cursor: col-resize;
}
</style>
