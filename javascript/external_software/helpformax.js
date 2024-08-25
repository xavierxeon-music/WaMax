autowatch = 1;

// inlets and outlets
inlets = 1;
setinletassist(0, "bang");

outlets = 1;
setoutletassist(0, "frontpatcher");

var path = "";

function bang() {

   if (!max.frontpatcher)
      return;

   var fp = max.frontpatcher.filepath;
   if (!fp)
      return;

   if (fp == path)
      return;

   if (fp == patcher.filepath)
      return;

   path = fp;

   outlet(0, fp);
}

