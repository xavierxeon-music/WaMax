autowatch = 1;

include("scale.js");
include("helper.js");

// inlets and outlets
inlets = 1;
setinletassist(0, "midinote, message");

outlets = 3;
setoutletassist(0, "midinote");
setoutletassist(1, "visu");
setoutletassist(2, "update");

var scales = new Global("Scales");
var scaleName = "main";

var settings = undefined;
var dict = undefined;


function getvalueof() {

   if (!settings)
      return;

   var text = JSON.stringify(settings);
   //Helper.debug("SAVE text", text);
   return text;
}

function setvalueof(value) {

   if (!value)
      return;

   //Helper.debug("LOAD text", value);
   settings = JSON.parse(value);
   if (dict) {
      dict.parse(JSON.stringify(settings));
      visu("resend");
   }
}

readDict.local = 1;
function readDict() {

   //Helper.debug("READ SETTINGS FROM DICT", dict);
   settings = JSON.parse(dict.stringify());

   var index = settings["scale"];
   if (index < 0) {

   }
   else {
      var scale = Scale.predefinedList[index];
      settings["noteActive"] = scale.notes;

      scaleName = settings["name"];
      if (!scales.target)
         scales.target = {};

      scales.target[scaleName] = scale;
   }


   writeDict();
}

writeDict.local = 1;
function writeDict() {

   //Helper.debug("WRITE SETTINGS TO DICT");
   dict.parse(JSON.stringify(settings));

   outlet(2, "bang");
   visu("resend");
}


function dictionary(dictName) {

   dict = new Dict(dictName);
   if (!settings) {
      readDict();
   }

   if (!settings["name"])
      settings["name"] = "main";

   if (!settings["noteActive"])
      settings["noteActive"] = [false, false, false, false, false, false, false, false, false, false, false, false];

   if (!settings["scale"])
      settings["scale"] = -1;

   //Helper.debug("LOAD DICT JS", dictName);
   writeDict();
}

function bang() {

   readDict();
}

function name(text) {

   scaleName = text;
}

function visu(command) {

   //Helper.debug("VISU", command);
   if ("resend" == command)
      outlet(1, settings["noteActive"]);
   else if ("clear" == command)
      outlet(1, [false, false, false, false, false, false, false, false, false, false, false, false]);
}

function msg_int(midiNote) {

   if (scales.target === undefined) {
      //Helper.debug("no target");
      outlet(0, midiNote);
      return;
   }

   if (scales.target[scaleName] === undefined) {
      //Helper.debug("no scale", scaleName);
      outlet(0, midiNote);
      return;
   }

   midiNote = scales.target[scaleName].closestMatch(midiNote);
   outlet(0, midiNote);
}


