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
      };
    },
    methods: {
      changeText(text: string) {
        this.labelText = text;
      },
    },
    mounted() {
        const container = this.$refs.container;
	const list = container.getElementsByTagName("div");

	if ((list.length % 2) != 1) {
	    alert("jse-vertical-split expects an odd number of elements");
	    return;
	}

        console.log("Sizes are");
	for (var i = 0; i < list.length; i++) {
	    var el = list[i];
	    // Find a more elegant way to do this?  Maybe insert
	    // the bars programatically?
	    if (el.getAttribute("class").includes("bar")) {
	        if ((i % 2) != 1) {
		    alert("Expected bar to have odd index");
		    return;
		}
                el.addEventListener('mousedown', (e) => {
                    this.panel = e.target.previousSibling;
                    this.next = e.target.nextSibling;

                    // Track all of the information we need to
		    // perform the movement.  Where did the mouse start?
		    // Where did the widths start?
                    this.mouseDown = true;
		    this.startPosX = e.clientX;
		    this.startPosY = e.clientY;
		    this.startWidth = this.panel.getBoundingClientRect().width;
	            this.nextWidth = this.next.getBoundingClientRect().width;

		    console.log(e);
                });
	    }
	    else {
		console.log(el.getBoundingClientRect());
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
    },
  };


</script>
