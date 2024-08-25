autowatch = 1;

// inlets and outlets
inlets = 1;
setinletassist(0, "gird");

outlets = 1;
setoutletassist(0, "color");

//////////////////////////////////////////

var inactiveColor = "off";
declareattribute("inactiveColor")

var activeColor = "off"
declareattribute("activeColor")

var x = null;
var y = null;
var miidKeys = [41, 42, 43, 44, 31, 32, 33, 34, 21, 22, 23, 24, 11, 12, 13, 14];

//////////////////////////////////////////

function loadbang() {

   bang();
}

function bang() {

   for (var x = 0; x < 4; x += 1) {
      for (var y = 0; y < 4; y += 1) {
         update(x, y, inactiveColor);
      }
   }
}

function list(nx, ny) {

   update(x, y, inactiveColor);

   x = nx;
   y = ny;
   update(nx, ny, activeColor);
}

update.local = 1;
function update(x, y, color) {

   if (null == x || null == y)
      return;

   var index = x + (4 * y);
   var midi = miidKeys[index];
   outlet(0, [midi, color]);
   //print(x, y, index, midi, color);
}