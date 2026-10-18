<template>
    <div class="horiz-container" ref="container">
        <slot></slot>
    </div>

</template>

<script setup lang="ts">
import { useTemplateRef, onMounted } from 'vue';

class Props {
    panel: HTMLElement;
    next: HTMLElement;
    mouseDown: boolean;
    startPosX: number;
    startPosY: number;
    startHeight: number;
    nextHeight: number;
    constructor(_panel: HTMLElement, _next: HTMLElement, clientX:number, clientY:number) {
        this.panel = _panel;
        this.next = _next;
	
        // Track all of the information we need to
        // perform the movement.  Where did the mouse start?
        // Where did the widths start?
        this.mouseDown = true;
        this.startPosX = clientX;
        this.startPosY = clientY;
        this.startHeight = this.panel.getBoundingClientRect().height;
        this.nextHeight = this.next.getBoundingClientRect().height;
    }
}

var props: Props | null = null;

const containerElement = useTemplateRef('container')!;

onMounted(() => {
	const list : HTMLElement[] = [];
        for (var cle of containerElement.value.children) {
	    list.push(cle);
	}
	for (var i = 0; i < list.length; i++) {
	    var el = list[i]!;
            if (i != 0) {
	        var bar = document.createElement("div");
		bar.setAttribute("class", "resize_bar_horizontal");
		el.parentElement!.insertBefore(bar, el);
                //container.insertBefore(bar, el);
		
                bar.addEventListener('mousedown', (e: MouseEvent) => {
                    const target: HTMLElement = e.target! as HTMLElement;
                    props = new Props(
		        target.previousElementSibling! as HTMLElement,
                        target.nextElementSibling! as HTMLElement,
			e.clientX, e.clientY);
                    e.preventDefault();
                });
	    }
	}
        list[list.length-1]!.style.flex = "1"; // the right panel will stretch to fit
	
        document.addEventListener('mousemove', (e) => {
            if (props == null) return;
	    
	    // We want to resize both the left and right
	    // panels so that we keep the total width the same.
	    var xval = props!.startHeight + (e.clientY - props!.startPosY);
	    var xval2 = props!.nextHeight - (e.clientY - props!.startPosY);
            props.panel.style.height = `${xval}px`;
	    props.next.style.height = `${xval2}px`;
        });
        document.addEventListener('mouseup', () => {
            props = null;
        });
});

</script>

<style>
.horiz-container {
  display: table;
  width: 100%;
  height: 75vh;
}
.resize_bar_horizontal {
  background-color: #3C3530;
  width: 100%;
  height: 5px;
  cursor: row-resize;
}
</style>
