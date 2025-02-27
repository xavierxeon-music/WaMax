// inspect

include("helper.js");

if (typeof this.Inspector === "undefined") {

   this.Inspector = class {

      constructor(object) {

         this.object = object;
      }

      compileAttributes() {

         var attributeObject = {};

         if (!this.object.getattrnames)
            return attributeObject;

         var keyList = this.object.getattrnames();
         for (var index in keyList) {
            var key = keyList[index];
            var value = this.object.getattr(key);
            attributeObject[key] = value;
         }

         return attributeObject;
      }

      compileBoxAttributes() {

         var boxattributeObject = {};

         if (!this.object.getboxattrnames)
            return boxattributeObject;

         var keyList = this.object.getboxattrnames();
         for (var index in keyList) {
            var key = keyList[index];
            var value = this.object.getboxattr(key);
            boxattributeObject[key] = value;
         }

         return boxattributeObject;
      }

      compileProperties() {

         var propertiesObject = {};

         for (var key in this.object) {
            var value = this.object[key];
            if ("object" === typeof value) {
               if (null === value)
                  propertiesObject[key] = "_NULL__";
               else if (undefined === value)
                  propertiesObject[key] = "__UNDFINED__";
               else
                  propertiesObject[key] = "__OBJECT__";
            }
            else
               propertiesObject[key] = value;
         }
         return propertiesObject;
      }

      static printObject(obj, tag, indent) {

         if (!indent)
            indent = 0;

         var name = tag ? tag : "#";
         for (var index = 0; index < indent; index++)
            name = ",.." + name;

         if (obj === null) {
            debug(name + " : NULL");
         }
         else if (obj === undefined) {
            debug(name + " : UNDEFINED");
         }
         else if (typeof obj == "number") {
            debug(name + " :" + obj, " (NUMBER)");
         }
         else if (typeof obj == "string") {
            debug(name + " :" + obj, "  (STRING)");
         }
         else if (typeof obj == "object") {
            debug(name + " is  (OBJECT)");
            for (var key in obj) {
               Inspector.printObject(obj[key], name + "," + key, indent + 1);
            }
         }
         else {
            debug(name + " is " + typeof obj);

         }
      }
   }
}
