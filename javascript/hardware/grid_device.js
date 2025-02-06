autowatch = 1;

include("color.js");

// inlets and outlets
inlets = 1;
setinletassist(0, "message");

outlets = 2;
setoutletassist(0, "midi");
setoutletassist(1, "rgb");

const lookup = new Dict("grid_mapping");

const redMidi = 128;
const greenMidi = 144;
const blueMidi = 160;

function parseColor(localId, name) {

   const color = new Color(name);
   const id = lookup.get("local_to_midi::" + localId);

   outlet(1, [color.red, color.green, color.blue, id]);

   midiRed = Math.floor(color.red / 2);
   midiGreen = Math.floor(color.green / 2)
   midBlue = Math.floor(color.blue / 2);

   outlet(0, [redMidi, id, midiRed]);
   outlet(0, [greenMidi, id, midiGreen]);
   outlet(0, [blueMidi, id, midBlue]);
}

function convertColor(id, name) {

   color = new Color(name);

   outlet(0, ["color", id, color.hex]);
}
