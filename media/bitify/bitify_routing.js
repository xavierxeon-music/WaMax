//
let route = [];

function routeIndex(col, row) {

   return col + (row * 8);
}


function toggleRoute(col, row) {

   let indexOn = routeIndex(col, row);
   if (route[indexOn])
      return; // can not toggle myself off

   for (let rowOff = 0; rowOff < 8; rowOff++) {

      let indexOff = routeIndex(col, rowOff);
      if (route[indexOff]) {
         route[indexOff] = 0;
         max.outlet("route", col, rowOff, 0);
         break;
      }
   }

   route[indexOn] = 1;
   max.outlet("route", col, row, 1);

   canvas.update();
}

function reset() {

   route = [];

   for (let col = 0; col < 8; col++) {

      for (let row = 0; row < 8; row++) {

         let active = (row == col) ? 1 : 0;
         route.push(active);

         max.outlet("route", col, row, active);
      }
   }

   canvas.update();
}

//
class RountingCanvas extends Canvas {

   constructor() {

      super(document.body, 180, 180);
      this.padding = 5;

      this.ctx.font = "12px Arial";
      this.ctx.textAlign = "left";

      this.element.addEventListener("pointerdown", (clickEvent) => {
         this.#clicked(clickEvent.offsetX, clickEvent.offsetY);
      });
   }

   update() {

      this.clear();

      const outlineColor = "#666666";
      const fullColor = "#ffffff";
      const arrowLeft = String.fromCharCode(parseInt('2b05', 16));
      const arrowRight = String.fromCharCode(parseInt('2b95', 16));

      this.ctx.fillStyle = fullColor;
      this.ctx.fillText("input " + arrowRight, 70, 175);


      this.ctx.save();
      this.ctx.translate(10, 110);
      this.ctx.rotate(-Math.PI / 2);
      this.ctx.fillText(arrowLeft + " output", 0, 0);
      this.ctx.restore();

      const padSize = 17;

      for (let col = 0; col < 8; col++) {

         let x = 15 + col * 20;

         for (let row = 0; row < 8; row++) {

            let y = 5 + row * 20;

            let index = routeIndex(col, row);
            let routeColor = route[index] ? fullColor : outlineColor;

            this.box(x, y, padSize, padSize, routeColor);
         }
      }
   }

   #clicked(x, y) {

      if (x < 15 || x >= 175)
         return;
      if (y < 5 || y > 165)
         return;

      let col = parseInt((x - 15) / 20);
      let row = parseInt((y - 5) / 20);
      toggleRoute(col, row);
   }
}

//
setupDocument(182, 1, 1);
let title = new Title("bitify routing");
title.setStyle("height", "20px");

let resetButton = new Button(title, "reset");
resetButton.onClicked(reset);
resetButton.move(135, 3);

let canvas = new RountingCanvas();
reset();
