//

class MessageBuffer {

   constructor(length, defaultValue) {

      this.length = length;
      this.defaultValue = defaultValue;
      this.index = 0;

      this.buffer = new Array(length).fill(defaultValue);
   }

   push(content) {
      this.buffer[this.index] = content;
      this.index = (this.index + 1) % this.length;
   }

   get(index) {
      let fetch = (this.index + index) % this.length;
      return this.buffer[fetch];
   }

   clear() {
      this.index = 0;
      this.buffer = new Array(this.length).fill(this.defaultValue);
   }
}

// 

let numberOfRows = 50;

const messageBuffer = new MessageBuffer(numberOfRows, ["", ""]);
const tableElements = new Array(length).fill(undefined);

function updateTable() {

   for (let index = 0; index < numberOfRows; index++) {

      let content = messageBuffer.get(index);
      let rows = tableElements[index];

      rows[0].innerHTML = content[0];
      rows[1].innerHTML = content[1];
   }
}

max.bindInlet('addMessage', addMessage);
function addMessage(text) {

   const now = new Date();
   let hh = now.getHours().toString();
   if (hh.length < 2)
      hh = "0" + hh;
   let mm = now.getMinutes().toString();
   if (mm.length < 2)
      mm = "0" + mm;
   let ss = now.getSeconds().toString();
   if (ss.length < 2)
      ss = "0" + ss;

   let time = hh + ":" + mm + ":" + ss;
   messageBuffer.push([time, text]);
   updateTable();
}

max.bindInlet('clearMessages', clearMessages);
function clearMessages() {

   messageBuffer.clear();

   updateTable();
}

max.bindInlet('resizeTable', resizeTable);
function resizeTable(width, height) {

   if (width < 250)
      width = 250;
   if (height < 150)
      height = 150;

   height -= 20; // title bar
   document.body.style["width"] = width.toString() + "px";
   scroll.forceHeigth(height.toString() + "px");

   //updateTable();
}



// gui
setupDocument(undefined, 1, 1);

// top
let title = new Title("console");

let nameEdit = new TextEdit(title);
nameEdit.onChange(nameChanged);
nameEdit.setStyle("width", "130px");

//title.buttonCount++;
title.addButton("clear", clearMessages);


// table
let scroll = new Div();
let table = new Table(scroll, ["60px", "auto"]);
for (let index = 0; index < numberOfRows; index++) {
   let rows = table.addRow(["", ""]);
   tableElements[numberOfRows - (1 + index)] = rows;
}

resizeTable(250, 200);
updateTable();

// name
max.bindInlet('setName', setName);
function setName(name) {

   nameEdit.setText(name);
}

function nameChanged(text) {

   max.outlet("updateName", text);
}
