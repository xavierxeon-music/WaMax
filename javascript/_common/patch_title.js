autowatch = 1;

// inlets and outlets
inlets = 1;
setinletassist(0, "me / top");

outlets = 1;
setoutletassist(0, "title");


function me() {

   outlet(0, patcher.name);
}

function top() {

   topPatcher = patcher;
   while (topPatcher.parentpatcher)
      topPatcher = topPatcher.parentpatcher;

   outlet(0, topPatcher.name);

}

