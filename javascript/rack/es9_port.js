autowatch = 1;

// inlets and outlets
inlets = 1;
setinletassist(0, "int/name");

outlets = 1;
setoutletassist(0, "id");

include("_portDevice.js");


//////////////////////////////////////////

// set up

var es9 = new PortDevice("es9");

var portIndex = undefined;
var portName = undefined;
var isSender = jsarguments[1];

//////////////////////////////////////////

function loadbang() {

   /*
   ES9
   (omnibus id) = (port)
   to computer: oni 3-14 =  1-12
   from computer (isSender): omni 3-10 = 1-8 (9-10 on device)
   */

   es9.register();
   es9.setup(3, 12, 3, 8);
}

function notifydeleted() {

   es9.removePortName(portIndex, isSender);
   es9.deregister();
}

function setIndex(index) {

   // turn off old index
   es9.removePortName(portIndex, isSender);

   var omniIndex = es9.omniIndex(index, isSender);
   portIndex = index;

   if (isSender) {
      outlet(0, ["set", omniIndex]);
   }
   else {
      outlet(0, ["set", 1, omniIndex]);
   }

   es9.addPortName(portIndex, portName, isSender);
}


function name(text) {

   portName = text;
   es9.addPortName(portIndex, portName, isSender);
}
