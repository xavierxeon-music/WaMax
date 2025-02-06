autowatch = 1;

include("color.js");

// inlets and outlets
inlets = 1;
setinletassist(0, "color");

outlets = 1;
setoutletassist(0, "rgb");

function lookup(name) {

   color = new Color(name);
   outlet(0, [color.red, color.green, color.blue]);
}