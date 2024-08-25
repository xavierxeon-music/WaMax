autowatch = 1;

// inlets and outlets
inlets = 1;
setinletassist(0, "bang");

outlets = 3;
setoutletassist(0, "input_id");
setoutletassist(1, "output_id");
setoutletassist(2, "start");

var audioDeviceDict = new Dict("audio_devices");

function bang() {

   var deviceName = audioDeviceDict.get("name");

   var inputId = 0;
   if (1 === audioDeviceDict.get("input_enabled"))
      inputId = audioDeviceDict.get("input::" + deviceName);

   var outputId = audioDeviceDict.get("output::" + deviceName);

   if (null === inputId || null === outputId)
      return;

   outlet(0, inputId);
   outlet(1, outputId);

   var start = audioDeviceDict.get("auto_start");
   outlet(2, start);

   // print("AUDIO for ", deviceName, ": in = ", inputId, ", out = ", outputId, " -> ", start ? "auto" : "manual");
}

