// inspect

var __defined__Inspector;
if (!__defined__Inspector) {
   __defined__Inspector = 1;

   class Inspector {

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
            print(name + " : NULL");
         }
         else if (obj === undefined) {
            print(name + " : UNDEFINED");
         }
         else if (typeof obj == "number") {
            print(name + " :" + obj, " (NUMBER)");
         }
         else if (typeof obj == "string") {
            print(name + " :" + obj, "  (STRING)");
         }
         else if (typeof obj == "object") {
            print(name + " is  (OBJECT)");
            for (var key in obj) {
               printObject(obj[key], name + "," + key, indent + 1);
            }
         }
         else {
            print(name + " is " + typeof obj);

         }
      }


   }
}