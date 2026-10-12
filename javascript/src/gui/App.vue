<template>
  <h1>You did it!</h1>
  <p>
    Visit <a href="https://vuejs.org/" target="_blank" rel="noopener">vuejs.org</a> to read the
    documentation
  </p>
  <Suspense>
      <editor/>
  </Suspense>
  <p>Editor is over now</p>
</template>

<style scoped></style>

<script setup lang="ts">
import editor from "./editor.vue";
</script>

