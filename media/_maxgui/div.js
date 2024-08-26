// div

class Div extends BaseElement {

   constructor(parent, value, alignment, id) {

      super("div", parent);

      if (id) {
         let span = createAndAppend("span", this.element);
         span.setAttribute("id", id);
         this.element = span;
      }

      if (value)
         this.setText(value.toString());

      if (alignment)
         this.element.style.textAlign = alignment;
   }

   setText(text) {
      this.element.innerHTML = text;
   }

   forceHeigth(height) {
      this.setStyle("height", height);
      this.setStyle("overflow-x", "hidden");
      this.setStyle("overflow-y", "auto");
   }

   makeHorizontal() {
      this.setStyle("float", "left");
      this.setStyle("clear", "none");
   }

   setGridLayout(layout, gap) {
      this.setStyle("display", "grid");
      this.setStyle("column-gap", gap ? gap.toString() + "px" : "5px");
      this.setStyle("grid-template-columns", Div.extractLayout(layout));
   }

   setColor(bgColor, fgColor) {
      area.setStyle("background-color", bgColor);
      if (fgColor)
         area.setStyle("color", fgColor);
   }

   static extractLayout(layout) {

      if (!layout)
         return undefined;

      let text = "";
      for (let index in layout) {
         if (0 != text.length)
            text += " ";

         let value = layout[index];
         if ("number" == typeof value)
            value = value.toString() + "px";

         text += value;
      }
      return text;
   }
}

class Spacer extends BaseElement {

   constructor(parent) {

      super("div", parent);

      this.element.className = "spacer";
   }
}

class Title extends BaseElement {

   constructor(text) {

      super("div");
      this.name = text;

      this.element.className = "title";
      this.element.innerText = this.name;

      this.buttonCount = 0;
   }

   showMessage(text) {
      this.element.innerHTML = this.name + " [" + text + "]";
   }

   clearMessage() {
      this.element.innerText = this.name;
   }

   addButton(text, receiver) {

      if (0 == this.buttonCount)
         new Spacer(this);

      this.buttonCount++;

      let resetButton = new Button(this, text);
      resetButton.onClicked(receiver);
   }
}
