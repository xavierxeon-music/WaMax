autowatch = 1;

// inlets and outlets
inlets = 1;
setinletassist(0, "message");

outlets = 2;
setoutletassist(0, "dac");
setoutletassist(1, "adc");

var global = new Global("AudioLoopback");

var stripName = "";
var stripId = 0;
var channel = undefined;

function loadbang() {

   if (!global.names)
      global.names = {};
}

function remove() {

   global.names[stripId] = "";
   if (global.update)
      global.update();
}

function name(text) {

   stripName = text;
   channelUpdate();
}

function setChannel(id) {

   if (id < 0)
      return;

   stripId = id;

   channel = 1 + (2 * (id - 1));
   channelUpdate();
}

channelUpdate.local = 1;
function channelUpdate() {

   if (!channel)
      return;

   global.names[stripId] = stripName;
   if (global.update)
      global.update();

   var channelIndex = channel + 32;

   outlet(0, channelIndex, channelIndex + 1);
   outlet(1, channelIndex, channelIndex + 1);
}

