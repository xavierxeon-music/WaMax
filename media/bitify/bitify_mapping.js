//
function sendReset() {

   max.outlet("reset", "bang");
}

//
class MappingCanvas extends Canvas {

   constructor() {

      super(document.body, 200, 200);
      this.padding = 5;

      this.element.addEventListener("pointerdown", (clickEvent) => {
         this.#clicked(clickEvent.layerX, clickEvent.layerY);
      });
      this.element.addEventListener("pointerup", (clickEvent) => {
         this.#released(clickEvent.layerX, clickEvent.layerY);
      });
      this.element.addEventListener("pointercancel", (clickEvent) => {
         this.#released(clickEvent.layerX, clickEvent.layerY);
      });
   }

   update() {

      this.clear();
   }

   #clicked(x, y) {

      debug(x, y);
   }

   #released(x, y) {

      debug(x, y);
   }
}

//
setupDocument(200, 1, 1);
let title = new Title("bitify mapping");
title.setStyle("height", "20px");

let resetButton = new Button(title, "reset");
resetButton.onClicked(sendReset);
resetButton.move(160, 3);

let canvas = new MappingCanvas();
canvas.update();
