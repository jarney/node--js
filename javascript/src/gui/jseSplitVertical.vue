<template>
  <div ref="container" class="container split">
      <div class="split__left"></div>
      <div class="split__bar"></div>
      <div class="split__left"></div>
      <div class="split__bar"></div>
      <div class="split__right"></div>
  </div>
</template>

<script lang="ts">

  export default {
    name: "jse-split-vertical",
    data() {
      return {
        elementSizes: [],
        xpos: {},
	currentlyMoving: null,
	currentlyMovingIndex: 0
      };
    },
    methods: {
      changeText(text: string) {
        this.labelText = text;
      },
    },
    mounted() {
        const container = this.$refs.container;
	console.log(container);
	const list = container.getElementsByTagName("div");
	console.log(list.length);

        var x_start = 0;
        var x_end = 0;

	for (var i = 0; i < list.length; i++) {
	    var el = list[i];
	    
	    const bbox = el.getBoundingClientRect();
	    console.log(bbox);
	    if (i == 0) {
	        x_start = bbox.x;
	    }
	    else {
	        x_start = x_end;
	    }
	    x_end = x_end + bbox.width;
	    this.xpos[i] = {start: x_start, end: x_end};
	    console.log(x_start + " - " + x_end);
	    if (el.getAttribute("class").includes("bar")) {
	        console.log("bar");
		const idx = i-1;
		const lastel = list[i-1];
                el.addEventListener('mousedown', (e) => {
                    console.log("Event mousedown on element " + idx);
                    mouse_is_down = true;
		    this.currentlyMoving = lastel;
		    this.currentlyMovingIndex = idx;
                });
	    }
	    else {
	        console.log("area");
	    }
	    this.elementSizes.push(list[i].offsetWidth);
    	    console.log("Got element " + list[i].getAttribute("class") + " " + this.elementSizes[i]);
	}
        const bar = document.querySelector('.split__bar');
        const left = document.querySelector('.split__left');
        let mouse_is_down = false;

        document.addEventListener('mousemove', (e) => {
            if (!mouse_is_down) return;
	    var w = this.xpos[this.currentlyMovingIndex].end - this.xpos[this.currentlyMovingIndex].start;
            console.log("Currently moving bar that goes from " +
            this.xpos[this.currentlyMovingIndex].end + " - " + this.xpos[this.currentlyMovingIndex].start);
            if (this.currentlyMovingIndex > 2) {
                var xval = e.clientX - this.xpos[this.currentlyMovingIndex].start - this.xpos[this.currentlyMovingIndex-2].end;
                this.currentlyMoving.style.width = `${xval}px`;
            }
            else {
                var xval = e.clientX - this.xpos[this.currentlyMovingIndex].start;
                this.currentlyMoving.style.width = `${xval}px`;
            }
        });
        document.addEventListener('mouseup', () => {
            mouse_is_down = false;
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
  width: 50%;
  background-color: #404040;
}

.split__right {
  flex: 1; /* the right panel will stretch the bg color*/
  background-color: #C5C6B6;
}

.split__bar {
  background-color: #3C3530;
  width: 15px;
  height: 100%;
  cursor: col-resize;
}
</style>
