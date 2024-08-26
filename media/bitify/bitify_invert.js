//
let invert = [0, 0, 0, 0, 0, 0, 0, 0];

function toggleInvert(xIndex) {

   invert[xIndex] ^= true;

   canvas.update();
   outputInvertValues();
}


function reset() {

   max.outlet("reset", "bang");
}

function outputInvertValues() {

   max.outlet("invert", ...invert);
}

max.bindInlet('set', setInvert);
function setInvert() {

   for (let col = 0; col < 8; col++)
      invert[col] = arguments[col];
   canvas.update();
}


//
class InvertCanvas extends Canvas {

   constructor() {

      super(document.body, 170, 35);
      this.padding = 5;

      this.element.addEventListener("pointerdown", (clickEvent) => {
         this.#clicked(clickEvent.offsetX, clickEvent.offsetY);
      });
   }

   update() {

      this.clear();

      const outlineColor = "#666600";
      const fullColor = "#ffff00";
      const padSize = 17;

      for (let col = 0; col < 8; col++) {

         let invertColor = invert[col] ? fullColor : outlineColor;

         let x = 5 + col * 20;
         this.box(x, 10, padSize, padSize, invertColor);
      }
   }

   #clicked(x, y) {

      if (x < 5 || x >= 165)
         return;
      if (y < 5 || y >= 35)
         return;

      let xIndex = parseInt((x - 5) / 20);
      toggleInvert(xIndex);
   }
}

//
setupDocument(172, 1, 1);
let title = new Title("bitify invert");
title.addButton("reset", reset);


let canvas = new InvertCanvas();
canvas.update();
