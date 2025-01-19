autowatch = 1;

// inlets and outlets
inlets = 1;
setinletassist(0, "bang");

outlets = 1;
setoutletassist(0, "sysex");

let fileName = null;
declareattribute("fileName");

function bang() {

   if (!fileName) {
      return;
   }

   var file = new File(fileName, "read");
   if (!file.isopen)
      return;

   for (let code of [240, 0, 102, 102])
      outlet(0, code);

   let code = 'P'.charCodeAt(0);
   outlet(0, code);

   for (let code of file.readbytes())
      outlet(0, code);

   file.close();

   const trail = [247];
   outlet(0, trail);
}

