autowatch = 1;

inlets = 1;
outlets = 2;

let resizeToContent = false;
let padding = 0;

function bang() {

   if (!resizeToContent)
      return;

   // patch where the wa.patch.bpatcher is included
   let contentPatch = patcher.parentpatcher;
   contentPatch.setattr("presentation", 1);

   let size = compileContentSize(contentPatch);
   let bpatcher = findBPatcher(contentPatch);

   if (!bpatcher)
      return;

   let boxSize = bpatcher.getboxattr("patching_rect");
   let changed = false;

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


   outlet(0, "bang");
}

function mode(value) {

   resizeToContent = (0 == value);
}

function getsize() {


   // patch where the wa.patch.bpatcher is included
   let contentPatch = this.patcher.parentpatcher;

   let bpatcher = findBPatcher(contentPatch);
   if (!bpatcher)
      return;

   let r = bpatcher.rect;
   let w = r[2] - r[0];
   let h = r[3] - r[1];

   outlet(1, w, h);
}


compileContentSize.local = 1;
function compileContentSize(contentPatch) {

   let top = -1;
   let left = -1;
   let right = -1;
   let bottom = -1;


   for (let obj = contentPatch.firstobject; obj !== null; obj = obj.nextobject) {

      let isPresentation = obj.getattr("presentation");
      if (0 === isPresentation)
         continue;

      let obj_rect = obj.getattr("presentation_rect");
      if (null === obj_rect || undefined === obj_rect)
         continue;

      let oLeft = obj_rect[0];
      let oTop = obj_rect[1];
      let oRight = oLeft + obj_rect[2];
      let oBottom = oTop + obj_rect[3];

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

   let width = padding + right - left;
   let height = padding + bottom - top;

   return [width, height];
}

findBPatcher.local = 1;
function findBPatcher(contentPatch) {

   for (let includingPatch = contentPatch.parentpatcher; includingPatch !== null; includingPatch = includingPatch.parentpatcher) {

      for (let child = includingPatch.firstobject; child !== null; child = child.nextobject) {

         if ("jpatcher" !== child.maxclass)
            continue;

         let patchName = child.getattr("name");
         if (patchName !== contentPatch.name)
            continue;

         return child;
      }
   }

   return undefined;
}
