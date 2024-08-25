autowatch = 1;

// inlets and outlets
inlets = 1;
setinletassist(0, "fileName");

outlets = 0;

//////////////////////////////////////////

function bang() {
   post("1", this.patcher, "\n");
   post("2", this.patcher.parentpatcher, "\n");
   post("3", this.patcher.parentpatcher, "\n");
   post(" ", "\n");
}

function write(baseName) {

   this.tag = function (object) {

      print(object.maxclass, object.varname);
      if ("patcher" !== object.maxclass)
         return;

      var inspector = new Inspector(object);

      var properties = inspector.compileProperties();
      for (key in properties) {
         var value = properties[key];
         print("*", key, value);
      }

      var boxAttr = inspector.compileBoxAttributes();
      for (key in boxAttr) {
         var value = boxAttr[key];
         print("+", key, value);
      }
   }

   var topPatcher = this.patcher;
   this.tag(topPatcher);
   while (topPatcher.parentpatcher) {
      topPatcher = topPatcher.parentpatcher;
      this.tag(topPatcher);
   }

   topPatcher.applydeep(this.tag);

}

