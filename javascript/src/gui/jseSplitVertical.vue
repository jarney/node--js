<template>
  <div ref="container" class="container split">
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
    startWidth: number;
    nextWidth: number;
    constructor(_panel: HTMLElement, _next: HTMLElement, clientX:number, clientY:number) {
        this.panel = _panel;
        this.next = _next;
	
        // Track all of the information we need to
        // perform the movement.  Where did the mouse start?
        // Where did the widths start?
        this.mouseDown = true;
        this.startPosX = clientX;
        this.startPosY = clientY;
        this.startWidth = this.panel.getBoundingClientRect().width;
        this.nextWidth = this.next.getBoundingClientRect().width;
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
		bar.setAttribute("class", "split__bar");
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
	    var xval = props!.startWidth + (e.clientX - props!.startPosX);
	    var xval2 = props!.nextWidth - (e.clientX - props!.startPosX);
            props.panel.style.width = `${xval}px`;
	    props.next.style.width = `${xval2}px`;
        });
        document.addEventListener('mouseup', () => {
            props = null;
        });
});

</script>

<style>
.split__bar {
  background-color: #3C3530;
  width: 15px;
  height: 100%;
  cursor: col-resize;
}
</style>
