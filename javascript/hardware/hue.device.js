autowatch = 1;

include("color.js");
include("helper.js");

// inlets and outlets
inlets = 1;
setinletassist(0, "message");

outlets = 1;
setoutletassist(0, "state");

/////////////////////////////
// vars / setup

var hue = new Global("hue");

var deviceName = undefined;
var reverseGradient = false;

/////////////////////////////
// function

stateChange.local = 1;
function stateChange(state) {

   var on = state["on"];
   outlet(0, on);
}


function init(_deviceName) {

   deviceName = _deviceName;

   if (!hue.stateChange)
      hue.stateChange = {};

   hue.stateChange[deviceName] = stateChange;
}

function deinit() {

   delete hue.stateChange[deviceName];
}


function on() {

   if (!hue.onOff)
      return;

   hue.onOff(deviceName, true);
}

function off() {

   if (!hue.onOff)
      return;

   hue.onOff(deviceName, false);
}

function color(hexColor, duration) {

   if (!hue.onOff)
      return;

   hue.coloronly(deviceName, hexColor, duration);
}

function colorbright(hexColor, duration) {

   if (!hue.onOff)
      return;


   hue.colorbright(deviceName, hexColor, duration);
}

function brightness(value, duration) {

   if (!hue.onOff)
      return;

   hue.brightness(deviceName, value, duration);
}

function reverse(value) {

   reverseGradient = value;
}

function gradient() {

   var duration = 0;
   var gradientList = [];
   for (var index = 0; index < arguments.length; index++) {
      var value = arguments[index];
      if ("string" == typeof value)
         gradientList.push(value);
      else if ("number" == typeof value) {
         duration = value;
      }
   }

   if (!hue.gradient)
      return;

   if (reverseGradient)
      gradientList = gradientList.reverse();

   hue.gradient(deviceName, gradientList, duration);
}