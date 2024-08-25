autowatch = 1;

// inlets and outlets
inlets = 1;
setinletassist(0, "int/name");

outlets = 1;
setoutletassist(0, "id");

include("_portDevice.js");


//////////////////////////////////////////

// set up

var es8 = new PortDevice("es8");

var portIndex = undefined;
var portName = undefined;
var isSender = jsarguments[1];

//////////////////////////////////////////

function loadbang() {

   /*
   ES8
   (omnibus id) = (port)
   to computer: oni 3-6 =  1-4
   from computer (isSender): omni 3-10 = 1-8
   */

   es8.register();
   es8.setup(3, 4, 3, 8);
}

function notifydeleted() {

   es8.removePortName(portIndex, isSender);
   es8.deregister();
}

function setIndex(index) {

   // turn off old index
   es8.removePortName(portIndex, isSender);

   var omniIndex = es8.omniIndex(index, isSender);
   portIndex = index;

   if (isSender) {
      outlet(0, ["set", omniIndex]);
   }
   else {
      outlet(0, ["set", 1, omniIndex]);
   }

   es8.addPortName(portIndex, portName, isSender);
}


function name(text) {

   portName = text;
   es8.addPortName(portIndex, portName, isSender);
}
