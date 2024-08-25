// push button

class PushButton extends BaseElement {

   static buttonMap = {};

   constructor(parent, id, x, y, halfSize) {

      super("div", parent);
      this.element.className = "pushbutton";
      this.move(x, y);

      this.isPad = false;
      this.id = id;

      if (halfSize)
         this.setStyle("height", "12px");

      this.element.addEventListener("pointerdown", (clickEvent) => {
         this.#clicked();
      });
      this.element.addEventListener("pointerup", (clickEvent) => {
         this.#released();
      });
      this.element.addEventListener("pointercancel", (clickEvent) => {
         this.#released();
      });

      this.element.addEventListener("pointerenter", (hoverEvent) => {
         title.showMessage(this.id);
      });
      this.element.addEventListener("pointerleave", (hoverEvent) => {
         title.clearMessage();
      });

   }

   #clicked() {
      max.outlet("midi", this.isPad ? "padClicked" : "buttonClicked", this.id, 127);
   }
   #released() {
      max.outlet("midi", this.isPad ? "padClicked" : "buttonClicked", this.id, 0);
   }
}

class PushColorButton extends PushButton {

   constructor(parent, id, x, y, halfSize) {

      super(parent, id, x, y, halfSize);

      PushButton.buttonMap[id] = this;

      if (halfSize)
         this.setShape("<rect x='1' y='5' width='14' height='4' stroke-width='0'>/");
      else
         this.setShape("<rect x='1' y='11' width='14' height='4' stroke-width='0'/>");
   }

   setShape(shape) {

      this.element.innerHTML = "<svg width='16px' heigth='16px'>" + shape + "</svg>";
      this.setColor("cccccc");
   }

   setColor(hexColor) {

      let svg = this.element.querySelector("svg");
      svg.setAttribute("style", "fill: #" + hexColor + ";")
   }
}

class PushPad extends PushButton {

   constructor(parent, id, x, y) {

      super(parent, id, x, y);
      this.element.className = "pushpad";

      this.isPad = true;

      PushButton.buttonMap[id] = this;
   }

   setColor(hexColor) {

      this.setStyle("background", "#" + hexColor);
   }
}

max.bindInlet('setColor', setColor);
function setColor(name, hexColor) {

   let button = PushButton.buttonMap[name];
   if (button)
      button.setColor(hexColor);
}

