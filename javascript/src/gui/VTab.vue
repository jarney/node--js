<script setup lang="ts">
import { inject } from 'vue'

defineProps<{
  title: string
}>()

const activeTab = inject('activeTab')
</script>

<template>
  <div v-show="title === activeTab">
    <slot></slot>
  </div>
</template>
