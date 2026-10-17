<template>
  <div ref="container" class="container split">
      <div class="split__left">content-1</div>
      <div class="split__bar"></div>
      <div class="split__right">content-2</div>
      <div class="split__bar"></div>
      <div class="split__right">content-3</div>
  </div>
</template>

<script lang="ts">

  export default {
    name: "jse-split-vertical",
    data() {
      return {
	currentlyMovingIndex: 0,
	containerRect: {},
	panels: []
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

        this.containerRect = container.getBoundingClientRect();
	if ((list.length % 2) != 1) {
	    alert("jse-vertical-split expects an odd number of elements");
	    return;
	}

        console.log("Sizes are");
	for (var i = 0; i < list.length; i++) {
	    var el = list[i];
	    
	    if (el.getAttribute("class").includes("bar")) {
	        if ((i % 2) != 1) {
		    alert("Expected bar to have odd index");
		    return;
		}
		const idx = (i-1)/2;
                el.addEventListener('mousedown', (e) => {
                    console.log("Event mousedown on element " + idx);
                    this.mouseDown = true;
		    this.currentlyMovingIndex = idx;
		    this.startPosX = e.clientX;
		    this.startPosY = e.clientY;
		    console.log(e);
                });
	    }
	    else {
		console.log(el.getBoundingClientRect());
	        this.panels.push(el);
	    }
	}
	this.panels[this.panels.length-1].style.flex = "1"; /* the right panel will stretch to fit */
	
        document.addEventListener('mousemove', (e) => {
            if (!this.mouseDown) return;
	    // We want to resize both the left and right
	    // panels so that we keep the total width the same.
	    const panel = this.panels[this.currentlyMovingIndex];
	    const next = this.panels[this.currentlyMovingIndex+1];
	    const panelSize = panel.getBoundingClientRect();
	    const nextSize = next.getBoundingClientRect();
	    var xval = panelSize.width + (e.clientX - this.startPosX);
	    var xval2 = nextSize.width - (e.clientX - this.startPosX);
	    this.startPosX = e.clientX;
            panel.style.width = `${xval}px`;
	    next.style.width = `${xval2}px`;
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
<style scoped>
.split__left {
  background-color: #404040;
}

.split__right {
  background-color: #C5C6B6;
}

.split__bar {
  background-color: #3C3530;
  width: 15px;
  height: 100%;
  cursor: col-resize;
}
</style>
