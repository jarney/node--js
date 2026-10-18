<template>
  <div class="my-8">
    <div class="flex gap-4 mb-4 tab-container">
      <div
        v-for="slot of $slots.default!()"
        :key="slot.props!.title"
        class="tab"
        :class="{
        'tab-active': activeTab === slot.props!.title,
      }"
        @click="activeTab = slot.props!.title"
      >
        {{ slot.props!.title }}
      </div>
    </div>
    <slot></slot>
  </div>
</template>
<script setup lang="ts">
import { ref, provide, useSlots } from 'vue'

const slots = useSlots()
const activeTab = ref(slots!.default!()[0]!.props!.title)

provide('activeTab', activeTab)
</script>

<style>
.tab-container {
  display: flex;
  flex-direction: row;
}
.tab {
    border: 1px solid #000;
    border-radius: 10px 10px 0px 0px;
    padding: 2px;
    background-color: #aaa;
    flex:1;
    align:center;
    text-align:center;
}
.tab-active {
    background-color: #fff;
}
</style>