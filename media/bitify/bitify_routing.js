//
let invert = [];
let route = [];

function routeIndex(xIndex, yIndex) {

   //return (xIndex * 8) + yIndex;
   return xIndex + (yIndex * 8);
}

function toggleInvert(xIndex) {

   invert[xIndex] ^= true;

   canvas.update();
}

function toggleRoute(xIndex, yIndex) {

   let index = routeIndex(xIndex, yIndex);
   if (route[index])
      return; // can not toggle myself off

   for (let row = 0; row < 8; row++) {

      let index = routeIndex(xIndex, row);
      route[index] = (row == yIndex) ? 1 : 0;
   }

   canvas.update();
}

function reset() {

   invert = [];
   route = [];

   for (let col = 0; col < 8; col++) {

      invert.push(0);

      for (let row = 0; row < 8; row++) {

         let active = (row == col) ? 1 : 0;
         route.push(active);
      }
   }

   canvas.update();
}

//
class RountingCanvas extends Canvas {

   constructor() {

      super(document.body, 170, 220);
      this.padding = 5;

      this.ctx.font = "12px Arial";
      this.ctx.textAlign = "left";

      this.element.addEventListener("pointerdown", (clickEvent) => {
         this.#clicked(clickEvent.offsetX, clickEvent.offsetY);
      });
   }

   update() {

      this.clear();

      var outlineColor = "#666666";
      var fullColor = "#ffffff";

      this.ctx.fillStyle = fullColor;
      this.ctx.fillText("invert", 5, 15);
      this.ctx.fillText("route", 5, 52);

      const padSize = 17;

      for (let col = 0; col < 8; col++) {

         let invertColor = invert[col] ? fullColor : outlineColor;

         let x = 5 + col * 20;
         this.box(x, 20, padSize, padSize, invertColor);

         for (let row = 0; row < 8; row++) {

            let y = 55 + row * 20;

            let index = routeIndex(col, row);
            let routeColor = route[index] ? fullColor : outlineColor;

            this.box(x, y, padSize, padSize, routeColor);
         }
      }
   }

   #clicked(x, y) {

      if (x < 5 || x >= 165)
         return;

      let xIndex = parseInt((x - 5) / 20);

      if (y >= 25 && y <= 45) {
         toggleInvert(xIndex);

      }
      else if (y >= 55 && y <= 215) {

         let yIndex = parseInt((y - 55) / 20);
         toggleRoute(xIndex, yIndex);
      }
   }
}

//
setupDocument(172, 1, 1);
let title = new Title("bitify routing");
title.setStyle("height", "20px");

let resetButton = new Button(title, "reset");
resetButton.onClicked(reset);
resetButton.move(130, 3);

let canvas = new RountingCanvas();
reset();
