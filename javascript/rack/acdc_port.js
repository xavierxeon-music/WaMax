autowatch = 1;

// inlets and outlets
inlets = 1;
setinletassist(0, "int/name");

outlets = 1;
setoutletassist(0, "id");

include("_portDevice.js");

//////////////////////////////////////////

// set up

var acdc = new PortDevice("acdc");

var portIndex = undefined;
var portName = undefined;
var isSender = jsarguments[1];

//////////////////////////////////////////

function loadbang() {

   /*
   ACDC
   (omnibus id) = (port)
   to computer: oni 17-20 =  1-4
   from computer (isSender): omni 11-14 = 1-4
   */

   acdc.register();
   acdc.setup(17, 4, 11, 4);
}

function notifydeleted() {

   acdc.removePortName(portIndex, isSender);
   acdc.deregister();
}

function setIndex(index) {

   // turn off old index
   acdc.removePortName(portIndex, isSender);

   var omniIndex = acdc.omniIndex(index, isSender);
   portIndex = index;

   if (isSender) {
      outlet(0, ["set", omniIndex]);
   }
   else {
      outlet(0, ["set", 1, omniIndex]);
   }

   acdc.addPortName(portIndex, portName, isSender);
}


function name(text) {

   portName = text;
   acdc.addPortName(portIndex, portName, isSender);
}
