autowatch = 1;

// inlets and outlets
inlets = 1;
setinletassist(0, "text");

outlets = 1;
setoutletassist(0, "text");

var global = new Global("AudioLoopback");
var dict = new Dict("loopback_names")

function loadbang() {

   global.update = updateDict;

   if (!global.names)
      global.names = {};

   for (var index = 1; index <= 16; index++) {

      var id = index.toString();
      if (!global.names[id])
         global.names[id] = "";
   }

   updateDict();
}

updateDict.local = 1;
function updateDict() {

   var data = JSON.stringify(global.names);
   dict.parse(data);
   outlet(0, "bang");
}


