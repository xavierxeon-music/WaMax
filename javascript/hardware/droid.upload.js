autowatch = 1;

include("helper.js")

// inlets and outlets
inlets = 1;
setinletassist(0, "bang");

outlets = 1;
setoutletassist(0, "sysex");

var fileName = null; // needs to be var ??
declareattribute("fileName");

function bang() {

   if (!fileName)
      return;

   var file = new File(fileName, "read");
   if (!file.isopen)
      return;

   //Helper.debug("reading", fileName, file.eof);

   for (let code of [240, 0, 102, 102])
      outlet(0, code);

   const code = 'P'.charCodeAt(0);
   outlet(0, code);

   let data = file.readbytes(file.eof);
   for (let code of data)
      outlet(0, code);

   file.close();

   const trail = [247];
   outlet(0, trail);
}

