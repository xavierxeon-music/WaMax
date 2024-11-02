//
let route = [1, 2, 3, 4, 5, 6, 7, 8];


function toggleRoute(col, row) {

   route[col] = row + 1;

   canvas.update();
   outputRouteValues();

}

function reset() {

   max.outlet("reset", "bang");

   canvas.update();
}

function outputRouteValues() {

   max.outlet("route", ...route);
}

max.bindInlet('set', setRoute);
function setRoute() {

   for (let col = 0; col < 8; col++)
      route[col] = arguments[col];
   canvas.update();
}


//
class RountingCanvas extends WebCanvas {

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

            let routeColor = (route[col] == row + 1) ? fullColor : outlineColor;
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
title.addButton("reset", reset);

let canvas = new RountingCanvas();
canvas.update();