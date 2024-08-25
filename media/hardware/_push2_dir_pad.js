//

class PushDirPad extends BaseElement {

   constructor(parent, idList, x, y) {

      super("div", parent);
      this.element.className = "pushdirpad";
      this.move(x, y);

      this.idList = idList;

      this.element.innerHTML = "<svg width='36px' heigth='28px'>\
                              <path d='M 0,0 L 36,28' stroke='#111111'/>\
                              <path d='M 0,28 L 36,0' stroke='#111111'/>\
                              </svg>";

      this.element.addEventListener("pointerdown", (clickEvent) => {
         let id = this.#getId(clickEvent.layerX, clickEvent.layerY);
         this.#clicked(id);
      });
      this.element.addEventListener("pointerup", (clickEvent) => {
         let id = this.#getId(clickEvent.layerX, clickEvent.layerY);
         this.#released(id);
      });
      this.element.addEventListener("pointercancel", (clickEvent) => {
         let id = this.#getId(clickEvent.layerX, clickEvent.layerY);
         this.#released(id);
      });

      this.element.addEventListener("pointermove", (hoverEvent) => {
         let id = this.#getId(hoverEvent.layerX, hoverEvent.layerY);
         title.showMessage(id);
      });
      this.element.addEventListener("pointerleave", (hoverEvent) => {
         title.clearMessage();
      });
   }

   #getId(x, y) {
      let xNorm = (x / this.element.offsetWidth) - 0.5;
      let yNorm = (y / this.element.offsetHeight) - 0.5;
      let isLeftRight = Math.abs(xNorm) > Math.abs(yNorm);

      if (isLeftRight) {
         if (xNorm > 0)
            return this.idList[2];
         else
            return this.idList[1];
      }
      else {
         if (yNorm > 0)
            return this.idList[3];
         else
            return this.idList[0];
      }
   }

   #clicked(id) {
      max.outlet("midi", "buttonClicked", id, 1);

   }
   #released(id) {
      max.outlet("midi", "buttonClicked", id, 0);
   }
}