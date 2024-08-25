autowatch = 1;

// inlets and outlets
inlets = 1;
setinletassist(0, "text");

outlets = 2;
setoutletassist(0, "on/off");
setoutletassist(1, "ticks");

var clockCounter = 0;

function clock() {

   clockCounter += 1;
   var ticks = 20 * clockCounter;
   outlet(1, ticks);
}

function start() {

   outlet(0, 1); // start

   clockCounter = 0;
   outlet(1, 0); // zero tick

   //clock();
   //clock();
   //clock();
}

function cont() {

   outlet(0, 1);
}

function stop() {

   outlet(0, 0);
}

function reset() {

   clockCounter = 0;
}


