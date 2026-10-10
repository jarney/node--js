<template>
  <h1>You did it!</h1>
  <p>
    Visit <a href="https://vuejs.org/" target="_blank" rel="noopener">vuejs.org</a> to read the
    documentation
  </p>

   <BaklavaEditor :view-model="baklava" />
   
   <p>Editor is over now</p>
</template>

<style scoped></style>

<script setup lang="ts">
import { defineNode, NodeInterface } from "@baklavajs/core";
import { BaklavaEditor, useBaklava, NumberInterface, SelectInterface } from "@baklavajs/renderer-vue";
import "@baklavajs/themes/dist/syrup-dark.css";

const MyNode = defineNode({
    type: "MyNode",
    inputs: {
        number1: () => new NumberInterface("Number", 1),
        number2: () => new NumberInterface("Number", 10),
        operation: () => new SelectInterface("Operation", "Add", ["Add", "Subtract"]).setPort(false),
    },
    outputs: {
        output: () => new NodeInterface("Output", 0),
    },
    calculate({ number1, number2, operation }) {
        let output: number;
        if (operation === "Add") {
            output = number1 + number2;
        } else {
            output = number1 - number2;
        }
        return { output };
    },
});
</script>

