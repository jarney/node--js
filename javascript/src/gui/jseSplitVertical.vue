<template>
  <div ref="container" class="container split">
      <slot></slot>
  </div>
</template>

<script lang="ts">
export default {
    name: "jse-split-vertical",
    data() {
      return {
          panel: null,
	  next: null,
          startPosX: 0,
	  startPosY: 0,
          startWidth: 99,
          nextWidth: 99
      }
    },
    methods: {
    },
    mounted() {
        const clist: Element[] = this.$refs.container!.children!;
	const list = [];
	for (var i = 0; i < clist.length; i++) {
	    list.push(clist[i]);
	}

	for (var i = 0; i < list.length; i++) {
	    var el = list[i];
            if (i != 0) {
	        var bar = document.createElement("div");
		bar.setAttribute("class", "split__bar");
		el.parentElement.insertBefore(bar, el);
                //container.insertBefore(bar, el);
		
                bar.addEventListener('mousedown', (e: Event) => {
                    this.panel = e.target!.previousSibling;
                    this.next = e.target!.nextSibling;

                    // Track all of the information we need to
		    // perform the movement.  Where did the mouse start?
		    // Where did the widths start?
                    this.mouseDown = true;
		    this.startPosX = e.clientX;
		    this.startPosY = e.clientY;
		    this.startWidth = this.panel.getBoundingClientRect().width;
	            this.nextWidth = this.next.getBoundingClientRect().width;

                    e.preventDefault();
                });
	    }
	}
	list[list.length-1].style.flex = "1"; /* the right panel will stretch to fit */
	
        document.addEventListener('mousemove', (e) => {
            if (!this.mouseDown) return;
	    // We want to resize both the left and right
	    // panels so that we keep the total width the same.
	    var xval = this.startWidth + (e.clientX - this.startPosX);
	    var xval2 = this.nextWidth - (e.clientX - this.startPosX);
            this.panel.style.width = `${xval}px`;
	    this.next.style.width = `${xval2}px`;
        });
        document.addEventListener('mouseup', () => {
            this.mouseDown = false;
        });
    },
    created() {
    },
    destroyed() {
    }
};

</script>

<style>
.split__bar {
  background-color: #3C3530;
  width: 15px;
  height: 100%;
  cursor: col-resize;
}
</style>
