<template>
  <h1>node--js node editor</h1>
  <Suspense>
      <editor ref="editor"/>
  </Suspense>
  <ul>Powered by:
  <li><a href="https://vuejs.org/" target="_blank" rel="noopener">vuejs.org</a>: General UI framework</li>
  <li><a href="https://baklavajs.org/" target="_blank" rel="noopener">baklavajs</a>: Node editor visuals</li>
  </ul>
</template>

<style scoped></style>

<script setup lang="ts">
import editor from "./editor.vue";

</script>


