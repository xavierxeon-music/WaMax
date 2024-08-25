autowatch = 1;

// inlets and outlets
inlets = 1;
setinletassist(0, "color");

outlets = 2;
setoutletassist(0, "hex color");
setoutletassist(1, "list color");

function list(red, green, blue, alpha) {

   var color = Color.fromRGB(red * 255, green * 255, blue * 255);
   outlet(0, color.hex);
   outlet(1, [red, green, blue, alpha]);
}

function color(value) {

   var color = Color(value);
   var red = color.red / 255.0;
   var green = color.green / 255.0;
   var blue = color.blue / 255.0;

   outlet(0, color.hex);
   outlet(1, [red, green, blue, 1.0]);
}

