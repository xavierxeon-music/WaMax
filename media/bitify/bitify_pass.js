//
let pass = [1, 1, 1, 1, 1, 1, 1, 1];

function toggleMute(xIndex) {

   pass[xIndex] ^= true;

   canvas.update();
   outputMuteValues();
}

function resetAll() {

   max.outlet("reset", false);
}

function resetNone() {

   max.outlet("reset", true);
}

function outputMuteValues() {

   max.outlet("pass", ...pass);
}

max.bindInlet('set', setMute);
function setMute() {

   for (let col = 0; col < 8; col++)
      pass[col] = arguments[col];
   canvas.update();
}

//
class MuteCanvas extends Canvas {

   constructor() {

      super(document.body, 170, 35);
      this.padding = 5;

      this.element.addEventListener("pointerdown", (clickEvent) => {
         this.#clicked(clickEvent.offsetX, clickEvent.offsetY);
      });
   }

   update() {

      this.clear();

      const outlineColor = "#666699";
      const fullColor = "#6666ff";
      const padSize = 17;

      for (let col = 0; col < 8; col++) {

         let passColor = pass[col] ? fullColor : outlineColor;

         let x = 5 + col * 20;
         this.box(x, 10, padSize, padSize, passColor);
      }
   }

   #clicked(x, y) {

      if (x < 5 || x >= 165)
         return;
      if (y < 5 || y >= 35)
         return;

      let xIndex = parseInt((x - 5) / 20);
      toggleMute(xIndex);
   }
}

//
setupDocument(172, 1, 1);
let title = new Title("bitify pass");
title.addButton("all", resetAll);
title.addButton("none", resetNone);

let canvas = new MuteCanvas();
canvas.update();
