// table

class Table extends BaseElement {

   constructor(parent, widths) {

      super("table", parent);

      if (widths)
         this.widths = widths;
      else {
         for (let index in headers)
            this.widths.push("auto");
      }
   }

   addHeader(headerList) {

      return this.#addRowInternal("th", headerList);
   }

   addRow(contentList) {

      return this.#addRowInternal("td", contentList);
   }

   #addRowInternal(tag, columnText) {

      let row = createAndAppend("tr", this.element);

      let rowElements = [];

      for (let index in columnText) {

         let text = columnText[index];

         let content = createAndAppend(tag, row);
         rowElements.push(content);

         content.innerText = text;
         content.style = "width:" + this.widths[index];
         content.align = "left";
      }

      return rowElements;

   }
}