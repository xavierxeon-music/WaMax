//
let mapping = [];

function reset() {

   mapping = [];

   for (let index = 0; index < 256; index++)
      mapping.push(index);

   canvas.update();
}

function changeMapping(x, y) {

   mapping[x] = y;

   canvas.update();
}

function smoothMapping() {

}

//
class MappingCanvas extends Canvas {

   constructor() {

      super(document.body, 270, 270);
      this.padding = 5;

      this.track = false;
      this.margin = 7;

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

      this.#change(x, y);
   }

   #move(x, y) {

      if (!this.track)
         return;

      this.#change(x, y);
   }

   #released(x, y) {

      this.track = false;
      smoothMapping();
   }

   #change(x, y) {

      x = parseInt(x - this.margin);
      if (x < 0 || x >= 256)
         return;

      y = parseInt(y - this.margin);
      y = (256 - y);
      if (y < 0 || y >= 256)
         return;

      changeMapping(x, y);
   }
}

//
setupDocument(271, 1, 1);
let title = new Title("bitify mapping");
title.addButton("reset", reset, true);

let canvas = new MappingCanvas();
reset();
