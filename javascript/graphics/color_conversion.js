
autowatch = 1;

include("helper.js");
include("color.js");

// inlets and outlets
inlets = 1;
setinletassist(0, "color");

outlets = 3;
setoutletassist(0, "hex color");
setoutletassist(1, "char color");
setoutletassist(2, "float color");

function fromFloat(red, green, blue) {

   let color = Color.fromRGB(red * 255, green * 255, blue * 255);
   colorOut(color);
}

function fromChar(red, green, blue) {

   let color = Color.fromRGB(red, green, blue);
   colorOut(color);
}

function color(value) {

   let color = new Color(value);
   colorOut(color);
}

colorOut.local = 1;
function colorOut(color) {

   outlet(0, color.hex);
   outlet(1, [color.red, color.green, color.blue]);

   let red = color.red / 255.0;
   let green = color.green / 255.0;
   let blue = color.blue / 255.0;
   outlet(2, [red, green, blue]);

}

