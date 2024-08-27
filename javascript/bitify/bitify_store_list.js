autowatch = 1;

// inlets and outlets
inlets = 1;
setinletassist(0, "text");

outlets = 1;
setoutletassist(0, "text");

//////////////////////////////////////////

const digit = jsarguments[1];
var settings = [digit, digit, digit, digit, digit, digit, digit, digit];

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

function reset(mode) {

   if (0 == mode)
      settings = [digit, digit, digit, digit, digit, digit, digit, digit];
   else
      settings = [1 - digit, 1 - digit, 1 - digit, 1 - digit, 1 - digit, 1 - digit, 1 - digit, 1 - digit];

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
      outlet(0, "value", "setvalue", col + 1, value);
   }

   if (!setDirty)
      return;

   var toppatcher = patcher;
   while (toppatcher.parentpatcher)
      toppatcher = toppatcher.parentpatcher;

   toppatcher.setattr("dirty", "1");
}
