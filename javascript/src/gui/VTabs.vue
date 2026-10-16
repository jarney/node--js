<template>
  <div class="my-8">
    <div class="flex gap-4 mb-4">
      <div
        v-for="slot of $slots.default!()"
        :key="slot.props!.title"
        class="cursor-pointer"
        :class="{
        'font-bold border-b-4 border-black': activeTab === slot.props!.title,
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

