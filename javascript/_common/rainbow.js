autowatch = 1;

include("color.js")

// inlets and outlets
inlets = 1;
setinletassist(0, "bang");

outlets = 2;
setoutletassist(0, "hexcolor");
setoutletassist(1, "rgb");

var counter = 0;
var offsetFactor = 0.0;
var maxCounter = 100;

var colorList = [];


// up = true
// donw = false
segment.local = 1;
function segment(redBase, redDiff, greenBase, greendDiff, blueBase, blueDiff) {

   for (var index = 0; index < 256; index += 1) {
      var red = redBase + (index * redDiff);
      var green = greenBase + (index * greendDiff);
      var blue = blueBase + (index * blueDiff);

      var color = Color.fromRGB(red, green, blue);
      // print(colorList.length, color.hex);
      colorList.push(color);
   }
}

init.local = 1;
function init() {

   var up = 1;
   var stay = 0;
   var down = -1;

   colorList = [];

   segment(255, stay, 0, up, 0, stay);
   segment(255, down, 255, stay, 0, stay);
   segment(0, stay, 255, stay, 0, up);
   segment(0, stay, 255, down, 255, stay);
   segment(0, up, 0, stay, 255, stay);
   segment(255, stay, 0, stay, 255, down);

   reset();
}

function loadbang() {

   init();
}

// get direct value
function msg_float(value) {

   value += offsetFactor;
   while (value < 0.0)
      value += 1.0;
   while (value > 1.0)
      value -= 1.0;

   var index = Math.round(value * 6 * 256);

   var color = colorList[index];

   outlet(0, color.hex);
   outlet(1, [color.red, color.green, color.blue]);

}


// advance on step
function bang() {

   var index = (counter * 6 * 256) / maxCounter;
   index = Math.round(index);
   var color = colorList[index];

   // print(counter, index, colorList.length, color.hex);

   outlet(0, color.hex);
   outlet(1, [color.red, color.green, color.blue]);

   counter += 1;
   if (counter == maxCounter)
      counter = 0;
}

function steps(value) {

   maxCounter = value;
   reset();
}

function offset(value) {

   offsetFactor = value;
   reset();
}

function reset() {

   counter = Math.round(maxCounter * offsetFactor);
}

