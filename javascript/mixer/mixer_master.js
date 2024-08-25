autowatch = 1;

// inlets and outlets
inlets = 1;
setinletassist(0, "message");

outlets = 0;

include("_mixer.js");

//////////////////////////////////////////

// set up

//////////////////////////////////////////

function unmute() {

   mixer.unMute();
}

function unsolo() {

   mixer.unSolo();
}

