autowatch = 1;

// inlets and outlets
inlets = 1;
setinletassist(0, "bang");

outlets = 1;
setoutletassist(0, "sysex");

const header = [240, 0, 102, 102, 'P'];
const trail = [247];

function bang() {



   var text = "";
   var file = new File(fileName, "read");
   if (!file.isopen)
      return null;

   while (file.position < file.eof) {
      text += file.readline();
   }
   file.close();
}

