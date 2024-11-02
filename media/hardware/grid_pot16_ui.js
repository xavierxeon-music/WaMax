//
let valueDict = {};
let nameDict = {};
let colorDict = {};

function sendReset() {

   max.outlet("reset", "bang");
}

max.bindInlet('gridInput', gridInput);
function gridInput(id, value) {

   value = value / 16382.0;
   valueDict[id] = value;
   canvas.update();
}

max.bindInlet('name', name);
function name(id, name) {

   nameDict[id] = name;
   canvas.update();
}

max.bindInlet('color', color);
function color(id, hexColor) {

   colorDict[id] = "#" + hexColor;
   canvas.update();
}


class GridCanvas extends WebCanvas {

   constructor() {

      super(document.body, 200, 200);
      this.padding = 5;

      this.ctx.font = "12px Arial";
      this.ctx.textAlign = "center";
   }

   update() {

      this.clear();

      var outlineColor = "#666666";
      var fullColor = "#ffffff";

      for (let row = 0; row < 4; row++) {
         for (let col = 0; col < 4; col++) {

            let x = 20 + (row * 50);
            let y = 20 + (col * 50);

            let index = ((4 - col) * 10) + (row + 1)

            // outer circle
            this.circle(x, y, 14, outlineColor);

            // knob indicator
            let fraction = valueDict[index];
            if (undefined == fraction)
               fraction = 0.0;

            if (fraction > 0.0)
               this.ring(x, y, 10, 7, fullColor, fraction, 0.5 * Math.PI);

            // inner circle
            this.circle(x, y, 9, outlineColor);

            // color inidicator
            let color = colorDict[index];
            if (undefined == color)
               color = "#000000";

            this.circle(x, y, 7, color);


            // name 
            let name = nameDict[index];
            let fontColor = fullColor;
            if (undefined == name) {
               name = index.toString();
               fontColor = outlineColor;
            }

            this.ctx.fillStyle = fontColor;
            this.ctx.fillText(name, x, y + 25);

         }
      }
   }
}

//
setupDocument(201, 1, 1);
let title = new Title("grid pot16");
title.addButton("reset", sendReset);

let canvas = new GridCanvas();
canvas.update();
