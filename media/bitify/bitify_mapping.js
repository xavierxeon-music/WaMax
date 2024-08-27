//
let mapping = [];

function reset() {

   max.outlet("reset");
}

function changeMapping(x, y) {

   mapping[x] = y;
   max.outlet("value", x, y);

   canvas.update();
}

function loadMapping() {

   max.outlet("load");
}

function saveMapping() {

   max.outlet("save");
}

max.bindInlet('set', setMapping);
function setMapping() {

   mapping = [];
   for (let col = 0; col < 256; col++)
      mapping[col] = arguments[col];
   canvas.update();
}

//
class MappingCanvas extends Canvas {

   constructor() {

      super(document.body, 270, 270);
      this.padding = 5;

      this.track = false;
      this.margin = 7;
      this.last = [-1, -1];

      this.setStyle("cursor", "crosshair");

      this.element.addEventListener("pointerdown", (clickEvent) => {
         this.#clicked(clickEvent.offsetX, clickEvent.offsetY);
      });

      this.element.addEventListener("pointermove", (clickEvent) => {
         this.#move(clickEvent.offsetX, clickEvent.offsetY);
      });

      this.element.addEventListener("pointerup", (clickEvent) => {
         this.#released(clickEvent.offsetX, clickEvent.offsetY);
      });
      this.element.addEventListener("pointerout", (clickEvent) => {
         this.#released(clickEvent.offsetX, clickEvent.offsetY);
      });
   }

   update() {

      this.clear();

      const outlineColor = "#666666";
      this.box(this.margin, this.margin, 256, 256, outlineColor);

      const fullColor = "#ff00ff";

      for (let index = 0; index < 256; index++) {

         let x = this.margin + index;
         let value = mapping[index];
         let y = this.margin + (256 - value);
         this.line(x, y, x, this.margin + 256, fullColor);
      }
   }

   #clicked(x, y) {

      this.track = true;

      [x, y] = this.#normalize(x, y);
      changeMapping(x, y);

      this.last = [x, y];
   }

   #move(x, y) {

      if (!this.track)
         return;

      [x, y] = this.#normalize(x, y);

      // interpolate last now
      let startX = this.last[0];
      let endX = x;
      let startY = this.last[1];
      let diffY = y - startY;

      if (startX > endX) {
         endX = startX;
         startX = x;

         startY = y;
         diffY *= -1;
      }

      diffY /= (endX - startX);

      for (let xi = startX; xi <= endX; xi++) {
         let yi = parseInt(startY + (xi - startX) * diffY);

         changeMapping(xi, yi);
      }

      this.last = [x, y];
   }

   #released(x, y) {

      this.track = false;
   }

   #normalize(x, y) {

      x = parseInt(x - this.margin);
      if (x < 0 || x >= 256)
         return [undefined, undefined];

      y = parseInt(y - this.margin);
      if (y < 0 || y >= 256)
         return [undefined, undefined];

      y = (256 - y);
      return [x, y];
   }
}

//
setupDocument(271, 1, 1);
let title = new Title("bitify mapping");
title.addButton("load", loadMapping);
title.addButton("save", saveMapping);
title.addButton("reset", reset);

let canvas = new MappingCanvas();
reset();
