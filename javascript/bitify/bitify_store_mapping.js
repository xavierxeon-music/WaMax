autowatch = 1;

// inlets and outlets
inlets = 1;
setinletassist(0, "text");

outlets = 1;
setoutletassist(0, "text");

var buf = new Buffer(jsarguments[1]);

//////////////////////////////////////////

var settings = {};

function getvalueof() {
   var text = JSON.stringify(settings);
   return text;
}

function setvalueof(value) {
   settings = JSON.parse(value);
}

//////////////////////////////////////////

function bang() {

   for (var index = 0; index < 256; index++) {

      buf.poke(1, index, 255 - index);
   }
}

