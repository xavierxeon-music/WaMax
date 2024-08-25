autowatch = 1;

// inlets and outlets
inlets = 1;
setinletassist(0, "text");

outlets = 1;
setoutletassist(0, "text");

function visu() {

   if (12 != arguments.length)
      return;

   for (var midiNote = 24; midiNote < 128; midiNote++) {
      var index = midiNote % 12;
      var active = arguments[index];

      outlet(0, [midiNote, active]);
   }
}

