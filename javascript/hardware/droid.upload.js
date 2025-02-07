autowatch = 1;

include("helper.js")

// inlets and outlets
inlets = 1;
setinletassist(0, "message");

outlets = 1;
setoutletassist(0, "sysex");

function upload(fileName) {

   if (!fileName)
      return;

   var file = new File(fileName, "read");
   if (!file.isopen)
      return;

   for (let code of [240, 0, 102, 102])
      outlet(0, code);

   const code = 'P'.charCodeAt(0);
   outlet(0, code);

   const comment = '#'.charCodeAt(0);
   const eol = '\n'.charCodeAt(0);
   const space = ' '.charCodeAt(0);
   let lastbyte = space;

   let incomment = false;

   let data = file.readbytes(file.eof);
   for (let code of data) {

      if (!incomment) {
         if (code == eol && lastbyte == eol) {
            continue;
         }

         if (code == comment) {
            incomment = true;
            continue;
         }
      }
      else if (incomment) {
         if (code == eol)
            incomment = false;
         continue;
      }

      lastbyte = code;
      if (code == space)
         continue;

      outlet(0, code);
   }

   file.close();

   const trail = [247];
   outlet(0, trail);
}

