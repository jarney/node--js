<template>
  <div>
    <pre>
    here?
        {{labelText}}
    </pre>
  </div>
</template>

<script lang="ts">
import {EventBus} from "./EventBus"

  export default {
    name: "Component1",
    data() {
      return {
        labelText: ""
      };
    },
    methods: {
      changeText(text: string) {
        this.labelText = text;
      },
    },
    mounted() {
        EventBus.get().on("changeText", (data: any) => {
            this.labelText = data.msg;
        });
    },
    created() {
        // Sets up the Event Bus listener using 
        // the custom event name and assosciates
        // it with a component method.
	// EventBus.$on("changeText", this.changeText);
      },
    destroyed() {
        // Removes Event Bus listener upon removal
        // of template from DOM.
        // EventBus.$off("changeText", this.changeText);
      },
  };
</script>
