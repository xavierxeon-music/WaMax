autowatch = 1;

// inlets and outlets
inlets = 1;
setinletassist(0, "text");

outlets = 1;
setoutletassist(0, "text");

//////////////////////////////////////////

var settings = [1, 2, 3, 4, 5, 6, 7, 8];

function getvalueof() {
   var text = JSON.stringify(settings);
   return text;
}

function setvalueof(value) {
   settings = JSON.parse(value);

   sendGen(false);
}

//////////////////////////////////////////

function sendUi() {

   outlet(0, "ui", "set", settings);
}

function reset() {

   settings = [1, 2, 3, 4, 5, 6, 7, 8];
   sendUi();
   sendGen(true);
}

function value() {

   for (var col = 0; col < 8; col++) {
      var value = parseInt(arguments[col]);
      settings[col] = value;
   }

   sendGen(true);
}

sendGen.local = 1;
function sendGen(setDirty) {


   for (var col = 0; col < 8; col++) {
      var value = settings[col];
      for (var row = 0; row < 8; row++) {
         outlet(0, "value", col, row, (row + 1 == value) ? 1 : 0);

      }
   }

   if (!setDirty)
      return;

   var toppatcher = patcher;
   while (toppatcher.parentpatcher)
      toppatcher = toppatcher.parentpatcher;

   toppatcher.setattr("dirty", "1");
}

