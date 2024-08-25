autowatch = 1;

// inlets and outlets
inlets = 1;
setinletassist(0, "value");

outlets = 1;
setoutletassist(0, "value");

//////////////////////////////////////////

// set up

var on = false;

//////////////////////////////////////////

function bang() {

   outlet(0, on ? 1 : 0);
}

function toggle(value) {

   // released
   if (0 === value)
      return;

   on ^= true;
   outlet(0, on ? 1 : 0);
}

function set(value) {

   on = (0 == value) ? false : true;
}





