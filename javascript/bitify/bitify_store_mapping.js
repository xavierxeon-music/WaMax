autowatch = 1;

// inlets and outlets
inlets = 1;
setinletassist(0, "text");

outlets = 1;
setoutletassist(0, "text");

var buf = new Buffer(jsarguments[1]);

//////////////////////////////////////////

var settings = [];

function getvalueof() {
   var text = JSON.stringify(settings);
   return text;
}

function setvalueof(value) {
   settings = JSON.parse(value);

   setBuffer(false);
}

//////////////////////////////////////////

function loadbang() {

   settings = [];
   for (var index = 0; index < 256; index++) {
      settings.push(index);
   }

}

function bang() {
   loadbang();
}

function sendUi() {

   outlet(0, "ui", "set", settings);
   setBuffer(false);
}

function value(x, y) {

   settings[x] = y;
   setBuffer(true);
}

function load(fileName) {

   settings = Json.readFile(fileName);

   sendUi();
   setBuffer(true);

}

function save(fileName) {

   Json.saveFile(settings, fileName);
}

function reset() {

   settings = [];
   for (var index = 0; index < 256; index++)
      settings.push(index);


   sendUi();
   setBuffer(true);
}

setBuffer.local = 1;
function setBuffer(setDirty) {

   for (var index = 0; index < 256; index++)
      buf.poke(1, index, settings[index]);


   if (setDirty)
      TopPatcher.markDirty(this);

}