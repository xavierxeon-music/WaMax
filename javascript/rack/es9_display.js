autowatch = 1;

// inlets and outlets
inlets = 1;
setinletassist(0, "text");

outlets = 1;
setoutletassist(0, "update");

include("_portDevice.js");

//////////////////////////////////////////

// set up

var es9 = new PortDevice("es9");
var portDict = new Dict("es9Display");

//////////////////////////////////////////

function loadbang() {

   es9.register(updateFunction);

   portDict.clear();
   updateFunction();
}

function notifydeleted() {

   es9.deregister();
}

updateFunction.local = 1;
function updateFunction() {

   // print("update"); es9.print();

   es9.copyTo(portDict);
   outlet(0, "bang");
}



