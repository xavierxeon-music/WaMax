// json


if (typeof this.Json === "undefined") {

   this.Json = class {

      static readFile = function (fileName) {

         let text = "";
         let file = new File(fileName, "read");
         if (!file.isopen)
            return null;

         while (file.position < file.eof) {
            text += file.readline();
         }
         file.close();

         let object = JSON.parse(text);
         return object;
      }

      static saveFile(object, fileName) {

         // TODO: delete old file
         let file = new File(fileName, "write");
         if (!file.isopen)
            return;

         let content = JSON.stringify(object);
         file.writestring(content);

         file.close();
      }

      static readDict(name) {

         let dict = new Dict(name);
         let text = dict.stringify();
         //delete dict;

         let object = JSON.parse(text);
         return object;
      }

      static writeDict(object, name) {

         let text = JSON.stringify(object);

         let dict = new Dict(name);
         dict.parse(text);
         //delete dict;
      }
   }
}

