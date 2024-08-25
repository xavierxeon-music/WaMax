autowatch = 1;

// inlets and outlets
inlets = 1;
setinletassist(0, "lookup/nearest");

outlets = 1;
setoutletassist(0, "frequency");

// data

include("_tuning.js")
var tuning = new Global("Tuning");

var myName = "main";

function setup(name, notesPerOctave, baseFrequency) {

   myName = name;
   tuning[myName] = new Tuning(notesPerOctave, baseFrequency);

   // print("SETUP", name, notesPerOctave, baseFrequency);
}

function lookup(index) {

   var frequency = tuning[myName].lookupIndex(index);
   outlet(0, frequency);
}

function nearest(value) {

   var frequency = tuning[myName].nearestFrequency(value);
   outlet(0, frequency);
}


