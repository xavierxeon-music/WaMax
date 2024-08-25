// 

class TextEdit extends BaseElement {

   constructor(parent) {

      super("input", parent);
      this.element.type = "text";
      this.element.className = "lineedit";
      this.callbackFunction = undefined;

      this.element.addEventListener("change", () => {
         if (this.callbackFunction)
            this.callbackFunction(this.element.value);
      });
   }

   setText(text) {

      this.element.value = text;
   }

   onChange(callbackFunction) {

      this.callbackFunction = callbackFunction;
   }
}