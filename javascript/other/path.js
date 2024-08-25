autowatch = 1;

// inlets and outlets
inlets = 1;
setinletassist(0, "path");

outlets = 1;
setoutletassist(0, "path");

function clean(path) {

   var index = path.indexOf(":");
   if (index >= 0) {
      var front = path.slice(0, index);
      var back = path.slice(index + 1);
      if ("Macintosh HD" == front) {
         path = back;
      }
      else {
         path = "/Volumes/" + front + back;
      }
   }

   print(index, front, back);
   outlet(0, path);
}

