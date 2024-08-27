// json

var Json = {}; // namesapce 

Json.readFile = function (fileName) {

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

Json.saveFile = function (object, fileName) {

   // TODO: delete old file
   var file = new File(fileName, "write");
   if (!file.isopen)
      return;

   var content = JSON.stringify(object);
   file.writestring(content);

   file.close();
}

Json.readDict = function (name) {

   var dict = new Dict(name);
   var text = dict.stringify();
   delete dict;

   var object = JSON.parse(text);
   return object;
}

Json.writeDict = function (object, name) {

   var text = JSON.stringify(object);

   var dict = new Dict(name);
   dict.parse(text);
   delete dict;
}