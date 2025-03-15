autowatch = 1;

// inlets and outlets
inlets = 1;
setinletassist(0, "text");

outlets = 1;
setoutletassist(0, "update");

include("_portDevice.js");

//////////////////////////////////////////

// set up

var es8 = new PortDevice("es8");
var portDict = new Dict("es8Display");

//////////////////////////////////////////

function loadbang() {

   es8.register(updateFunction);

   portDict.clear();
   updateFunction();
}

function notifydeleted() {

   es8.deregister();
}

updateFunction.local = 1;
function updateFunction() {

   // print("update"); es8.print();

   es8.copyTo(portDict);
   outlet(0, "bang");
}



