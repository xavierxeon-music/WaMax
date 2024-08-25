autowatch = 1;

// inlets and outlets
inlets = 1;
setinletassist(0, "mesage");

outlets = 2;
setoutletassist(0, "submatrix");
setoutletassist(1, "[off, meanColor, distance]");

// active
var minColor = new Color("black");
var maxColor = new Color("white");
var meanColor = new Color("black");
var targetColor = new Color("red");
var colorDistance = 0;

// box
var width = 1;
var height = 1;
var px = 0;
var py = 0;
var rx = 1;
var ry = 1;

function min(a, r, g, b) {

   minColor = Color.fromRGB(r, g, b);
   testAllColorsOff();
}

function mean(a, r, g, b) {

   meanColor = Color.fromRGB(r, g, b);
   testAllColorsOff();
}

function max(a, r, g, b) {

   maxColor = Color.fromRGB(r, g, b);
   testAllColorsOff();
}

function offColor(hexColor) {

   targetColor = new Color(hexColor);
   testAllColorsOff();
}

function offDistance(value) {

   colorDistance = value;
   testAllColorsOff();
}

function bBox(name, value) {

   if ("width" == name)
      width = Math.round(value);
   else if ("height" == name)
      height = Math.round(value);
   else if ("px" == name)
      px = Math.round(value);
   else if ("py" == name)
      py = Math.round(value);
   else if ("rx" == name)
      rx = Math.round(value);
   else if ("ry" == name)
      ry = Math.round(value);

   var cx = clamp(px, rx, width - rx);
   var cy = clamp(py, ry, height - ry);

   var minx = cx - rx;
   var miny = cy - ry;
   outlet(0, ["offset", minx, miny]);
   outlet(0, ["dim", 2 * rx, 2 * ry]);
}



clamp.local = 1;
function clamp(num, a, b) {

   if (num < a)
      num = a;
   else if (num > b)
      num = b;

   return num;
}

testAllColorsOff.local = 1;
function testAllColorsOff() {

   var off = 1;

   // not uniform
   var dist = minColor.distance(maxColor)
   if (dist > 2 * colorDistance) {
      off = 0;
   }
   else {
      // not close to target
      dist = minColor.distance(targetColor);
      if (dist > colorDistance) {
         off = 0;
      }
   }

   outlet(1, [off, meanColor.hex, dist]);
}