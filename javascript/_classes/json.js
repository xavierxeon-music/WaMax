// json


if (typeof this.Json === "undefined") {

   this.Json = class {

      static readFile = function (fileName) {

         var text = "";
         var file = new File(fileName, "read");
         if (!file.isopen)
            return null;

         while (file.position < file.eof) {
            text += file.readline();
         }
         file.close();

         var object = JSON.parse(text);
         return object;
      }

      static saveFile(object, fileName) {

         // TODO: delete old file
         var file = new File(fileName, "write");
         if (!file.isopen)
            return;

         var content = JSON.stringify(object);
         file.writestring(content);

         file.close();
      }

      static readDict(name) {

         var dict = new Dict(name);
         var text = dict.stringify();
         //delete dict;

         var object = JSON.parse(text);
         return object;
      }

      static writeDict(object, name) {

         var text = JSON.stringify(object);

         var dict = new Dict(name);
         dict.parse(text);
         //delete dict;
      }
   }
}

