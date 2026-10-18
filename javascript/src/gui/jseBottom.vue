<template>
    <div>
        <VTabs>
            <VTab title="Source">
                <div style="height: 90px; overflow-y: scroll;">
                    <TextBox></TextBox>
                </div>
            </VTab>
            <VTab title="Output">
                  XML of the graph (module) goes here.
                  This is a large scroll area with a 'copy' button.
            </VTab>
        </VTabs>
    </div>
</template>

<script setup lang="ts">
import TextBox from "./TextBox.vue";
import VTab from "./VTab.vue";
import VTabs from "./VTabs.vue";
</script>

<script lang="ts">
export default {
    name: "jse-bottom",
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
