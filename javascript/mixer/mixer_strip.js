autowatch = 1;

// inlets and outlets
inlets = 1;
setinletassist(0, "message");

outlets = 3;
setoutletassist(0, "volume");
setoutletassist(1, "unmute");
setoutletassist(2, "unsolo");


include("_mixer.js");

//////////////////////////////////////////

// set up

var isMute = false;
var isSolo = false;
var patcherName = null;

mixerHandle.local = 1;
function mixerHandle(idList, unmute) {

   //print("mixerHandle", idList.length, unmute);

   if (unmute) {

      isMute = false;
      outlet(1, 0);
   }


   isSolo = false;

   for (var index = 0; index < idList.length; index++) {
      if (idList[index] == patcherName) {
         isSolo = true;
         break;
      }
   }

   if (!isSolo)
      outlet(2, 0);

   updateVolumeOutlet();
}

setup.local = 1;
function setup() {

   patcherName = patcher.name + "_" + jsarguments[1];
   mixer.addHandle(mixerHandle);
}

//////////////////////////////////////////

updateVolumeOutlet.local = 0;
function updateVolumeOutlet() {

   if (isMute) {

      outlet(0, 0);
      return;
   }

   if (mixer.hasSolo() && !isSolo) {
      outlet(0, 0);
      return;

   }

   outlet(0, 1);
}

function loadbang() {
   setup();
}

function freebang() {
   mixer.removeHandle(mixerHandle);
}

function mute(value) {

   isMute = (1 == value);
   updateVolumeOutlet();
}

function solo(value) {

   // isSolo will be set via mixerHandler
   if (1 == value)
      mixer.enableSolo(patcherName);
   else
      mixer.disableSolo(patcherName);
}
