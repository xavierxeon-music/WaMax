//

var __defined__TopPatcher;
if (!__defined__TopPatcher) {
   __defined__TopPatcher = 1;

   class TopPatcher {

      static getPresentationRectanlge(object) {

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

      static markDirty(object) {

         var topPatcher = object.patcher;
         while (topPatcher.parentpatcher)
            topPatcher = topPatcher.parentpatcher;

         topPatcher.setattr("dirty", "1");
      }
   }
}
