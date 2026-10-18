<template>
    <div class="nodejs-right-sidebar">
        When a node is selected, detailed information about the node appears here.
        <ul>
            <li>* Node level metadata</li>
            <li>* Node level metadata</li>
        </ul>
        When a node is not selected, detailed information about the graph appears here.
        <ul>
            <li>* Graph level metadata</li>
        </ul>
    </div>
</template>

<script setup lang="ts">
import VTabs from './VTabs.vue'
import VTab from './VTab.vue'
</script>

<script lang="ts">
export default {
    name: "jse-right-sidebar",
    data() {
      return {
      };
    },
    methods: {
    },
    mounted() {
    },
    created() {
    },
    destroyed() {
    }
};


</script>
