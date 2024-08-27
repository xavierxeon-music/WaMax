//

var TopPatcher = {}; // namespace

TopPatcher.getPresentationRectanlge = function (object) {

   if (!object)
      return null;

   var objectPatchName = object.patcher.name;

   var topPatcher = object.patcher;
   while (topPatcher.parentpatcher) {
      topPatcher = topPatcher.parentpatcher;
   }

   for (var child = topPatcher.firstobject; child !== null; child = child.nextobject) {

      if ("patcher" !== child.maxclass)
         continue;

      var patchName = child.getattr("name");
      if (patchName !== objectPatchName)
         continue;

      var box_rect = child.getboxattr("presentation_rect");
      return box_rect;
   }

   return null;
}

TopPatcher.markDirty = function (object) {

   var topPatcher = object.patcher;
   while (topPatcher.parentpatcher)
      topPatcher = topPatcher.parentpatcher;

   topPatcher.setattr("dirty", "1");
}