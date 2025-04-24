autowatch = 1;

include("helper.js");

// inlets and outlets
inlets = 1;
setinletassist(0, "bang, dictionary");

outlets = 3;
setoutletassist(0, "input_id");
setoutletassist(1, "output_id");
setoutletassist(2, "start");

let audioDeviceDict = {};

function msg_dictionary(data) {
   audioDeviceDict = data;
}

function bang() {

   let deviceName = audioDeviceDict["name"];
   //Helper.debug("deviceName", deviceName);

   let inputId = 0;
   if (1 === audioDeviceDict["input_enabled"]) {
      let inputDict = audioDeviceDict["input"];
      for (let key in inputDict) {
         //Helper.debug("input", key, deviceName);
         if (key.includes(deviceName)) {
            inputId = inputDict[key];
            break;
         }
      }
   }

   let outputId = 0;
   let outputDict = audioDeviceDict["output"];
   for (let key in outputDict) {
      if (key.includes(deviceName)) {
         outputId = outputDict[key];
         break;
      }
   }

   if (null === inputId || null === outputId)
      return;

   outlet(0, inputId);
   outlet(1, outputId);

   let start = audioDeviceDict["auto_start"];
   outlet(2, start);

   // print("AUDIO for ", deviceName, ": in = ", inputId, ", out = ", outputId, " -> ", start ? "auto" : "manual");
}

