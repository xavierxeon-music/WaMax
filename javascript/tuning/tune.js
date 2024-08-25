autowatch = 1;

// inlets and outlets
inlets = 1;
setinletassist(0, "lookup/nearest");

outlets = 2;
setoutletassist(0, "frequency");
setoutletassist(1, "notesPerOctave");

// data

include("_tuning.js")
var tuning = new Global("Tuning");

var myName = "main";

var octave = 0;
declareattribute("octave");


function setName(name) {

   myName = name;
}

function bang() {

   if (tuning[myName] == undefined)
      return;

   outlet(1, tuning[myName].notesPerOctave);

}

function lookup(index) {

   if (tuning[myName] == undefined)
      return 0.0;

   index += (octave * tuning[myName].notesPerOctave);

   var frequency = tuning[myName].lookupIndex(index);
   outlet(0, frequency);
}

function nearest(value) {

   if (tuning[myName] == undefined)
      return 0.0;

   var frequency = tuning[myName].nearestFrequency(value);
   outlet(0, frequency);
}


