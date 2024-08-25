//
let mute = [];

function toggleMute(xIndex) {

   mute[xIndex] ^= true;
   canvas.update();
}

function reset() {

   mute = [0, 0, 0, 0, 0, 0, 0, 0];
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
title.setStyle("height", "20px");

let resetButton = new Button(title, "reset");
resetButton.onClicked(reset);
resetButton.move(125, 3);

let canvas = new MuteCanvas();
reset();
