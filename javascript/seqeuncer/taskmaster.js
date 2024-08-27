autowatch = 1;

// inlets and outlets
inlets = 1;
setinletassist(0, "message");

outlets = 1;
setoutletassist(0, "event");

var schedule = null;


function load(fileName) {

   schedule = Json.readFile(fileName);
}

function time(bars, beats) {

   if (!schedule)
      return;

   var key = bars + "." + beats;
   eventList = schedule[key]
   if (undefined == eventList)
      return;

   for (index in eventList) {

      var event = eventList[index];
      outlet(0, event);
   }
}
