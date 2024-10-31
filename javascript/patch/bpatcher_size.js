autowatch = 1;

inlets = 1;
outlets = 1;


var isAutomatic = false;
var padding = 0;

function bang() {

   var size = compileContentSize();

   var bpatcherList = findBPatchers();

   if (!isAutomatic)
      return;

   for (var index = 0; index < bpatcherList.length; index++) {

      var bpatcher = bpatcherList[index];
      if (null === bpatcher)
         return;

      var boxSize = bpatcher.getboxattr("patching_rect");
      var changed = false;

      if (boxSize[2] != size[0]) {
         boxSize[2] = size[0];
         changed = true;
      }

      if (boxSize[3] != size[1]) {
         boxSize[3] = size[1];
         changed = true;
      }
      if (changed)
         bpatcher.setboxattr("patching_rect", boxSize);
   }

   outlet(0, "bang");
}

function automatic(value) {

   isAutomatic = (1 == value);
}

function compileContentSize() {

   var top = -1;
   var left = -1;
   var right = -1;
   var bottom = -1;

   // patch where the wa.patch.bpatcher is included
   var contentPatch = patcher.parentpatcher;

   for (var obj = contentPatch.firstobject; obj !== null; obj = obj.nextobject) {

      var isPresentation = obj.getattr("presentation");
      if (0 === isPresentation)
         continue;

      var obj_rect = obj.getattr("presentation_rect");
      if (null === obj_rect || undefined === obj_rect)
         continue;

      var oLeft = obj_rect[0];
      var oTop = obj_rect[1];
      var oRight = oLeft + obj_rect[2];
      var oBottom = oTop + obj_rect[3];

      if (-1 === left || left > oLeft) {
         left = oLeft;
      }
      if (-1 === top || top > obj_rect[1]) {
         top = obj_rect[1];
      }
      if (-1 === right || right < oRight) {
         right = oRight;
      }
      if (-1 === bottom || bottom < oBottom) {
         bottom = oBottom;
      }

   }

   var width = padding + right - left;
   var height = padding + bottom - top;

   return [width, height];
}
compileContentSize.local = 1;

function findBPatchers() {

   // patch where the wa.patch.bpatcher is included
   var contentPatch = patcher.parentpatcher;

   var bpatcherList = [];

   // the patch that includes content patch
   var includingPatch = contentPatch.parentpatcher;
   if (null === includingPatch)
      return bpatcherList;

   contentPatch.setattr("presentation", 1);

   for (var child = includingPatch.firstobject; child !== null; child = child.nextobject) {

      if ("patcher" !== child.maxclass)
         continue;

      var patchName = child.getattr("name");
      if (patchName !== contentPatch.name)
         continue;

      bpatcherList.push(child);
   }
   return bpatcherList;
}
findBPatchers.local = 1;
