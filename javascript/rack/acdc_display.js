autowatch = 1;

// inlets and outlets
inlets = 1;
setinletassist(0, "text");

outlets = 1;
setoutletassist(0, "update");

include("_portDevice.js");

//////////////////////////////////////////

// set up

var acdc = new PortDevice("acdc");
var portDict = new Dict("acdcDisplay");

//////////////////////////////////////////

function loadbang() {

   acdc.register(updateFunction);

   portDict.clear();
   updateFunction();
}

function notifydeleted() {

   acdc.deregister();
}

updateFunction.local = 1;
function updateFunction() {

   // print("update"); acdc.print();

   acdc.copyTo(portDict);
   outlet(0, "bang");
}



