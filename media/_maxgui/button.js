// button

class Button extends BaseElement {

   constructor(parent, text) {

      super("button", parent);
      this.text = text;
      this.altText = undefined;
      this.callbackFunction = undefined;
      this.element.innerHTML = text;

      this.element.className = "";

      this.element.addEventListener("click", (clickEvent) => {
         this.#clicked();
      });

   }

   setToggle(altText) {

      this.altText = altText;
   }

   onClicked(callbackFunction) {

      this.callbackFunction = callbackFunction;
   }

   #clicked() {

      if (!this.altText)
         this.#callback(false);
      else {
         debug("toggle");
         // let wasDown = this.element.className.text.includes("active");
         if (this.element.className) {
            this.element.className = "";
            this.element.innerHTML = this.text;
            this.#callback(false);
         }
         else {
            this.element.className = "active";
            this.element.innerHTML = this.altText;
            this.#callback(true);

         }
      }
   }

   #callback(enabled) {

      if (!this.callbackFunction)
         return;

      this.callbackFunction(enabled);
   }
}
