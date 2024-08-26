//
let mute = [0, 0, 0, 0, 0, 0, 0, 0];

function toggleMute(xIndex) {

   mute[xIndex] ^= true;

   canvas.update();
   outputMuteValues();
}

function reset() {

   max.outlet("reset", "bang");
}

function outputMuteValues() {

   max.outlet("mute", ...mute);
}

max.bindInlet('set', setMute);
function setMute() {

   for (let col = 0; col < 8; col++)
      mute[col] = arguments[col];
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

      const outlineColor = "#444466";
      const fullColor = "#6666ff";
      const padSize = 17;

      for (let col = 0; col < 8; col++) {

         let muteColor = mute[col] ? fullColor : outlineColor;

         let x = 5 + col * 20;
         this.box(x, 10, padSize, padSize, muteColor);
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
let title = new Title("bitify mute");
title.addButton("reset", reset);

let canvas = new MuteCanvas();
canvas.update();

