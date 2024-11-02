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

let numberOfColumns = 50;

const messageBuffer = new MessageBuffer(numberOfColumns, ["", ""]);
const tableElements = new Array(length).fill(undefined);

function updateTable() {

   for (let index = 0; index < numberOfColumns; index++) {

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


// gui
setupDocument(250, 1, 1);

// top
let title = new Title("console");

let nameEdit = new TextEdit(title);
nameEdit.onChange(nameChanged);
nameEdit.setStyle("width", "130px");

//title.buttonCount++;
title.addButton("clear", clearMessages);


// table
let scroll = new Div();
scroll.forceHeigth("145px");

let table = new Table(scroll, ["60px", "auto"]);
for (let index = 0; index < numberOfColumns; index++) {
   let rows = table.addRow(["", ""]);
   tableElements[numberOfColumns - (1 + index)] = rows;
}

updateTable();

// name
max.bindInlet('setName', setName);
function setName(name) {

   nameEdit.setText(name);
}

function nameChanged(text) {

   max.outlet("updateName", text);
}
