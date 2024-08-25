autowatch = 1;

// inlets and outlets
inlets = 1;
setinletassist(0, "message");

outlets = 1;
setoutletassist(0, "matrix");

var window_width = 1;
var window_height = 1;
var rect_x = 0;
var rect_y = 0;
var rect_width = 0;
var rect_height = 0;

function wdim(w, h) {

   if (window_width == w && window_height == h)
      return;

   window_width = w;
   window_height = h;

   create();
}

function dim(w, h) {

   if (rect_width == w && rect_height == h)
      return;

   rect_width = w;
   rect_height = h;

   create();
}

function offset(x, y) {

   if (rect_x == x && rect_y == y)
      return;

   rect_x = x;
   rect_y = y;

   create();
}

normalize.local = 1;
function normalize(point, maxLength) {

   return (2 * point / maxLength) - 1.0;
}

create.local = 1;
function create() {

   var left = normalize(rect_x + 0, window_width);
   var right = normalize(rect_x + rect_width, window_width);
   var top = normalize(window_height - (rect_y - 0), window_height);
   var bottom = normalize(window_height - (rect_y + rect_height), window_height);

   outlet(0, ["setcell", 0, "val", left, top, 1]);
   outlet(0, ["setcell", 1, "val", right, top, 1]);
   outlet(0, ["setcell", 2, "val", right, bottom, 1]);
   outlet(0, ["setcell", 3, "val", left, bottom, 1]);
}
