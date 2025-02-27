// paths


if (typeof this.Paths === "undefined") {

   this.Paths = class {

      static home() {

         let homeFolder = new File("~");
         post("Home folder path: " + homeFolder.pathname + "\n");
         homeFolder.close();

         return homeFolder.pathname;
      }

      static topPatcher(this_patcher) {

         let top_patcher = this_patcher;
         while (top_patcher.parentpatcher) {
            top_patcher = top_patcher.parentpatcher;
         }

         let file = new File(top_patcher.filepath)
         let path = file.foldername;
         file.close();

         return path;
      }

      static exists(path) {

         let file = new File(path);
         let exists = file.isopen;
         file.close();

         return exists;
      }
   }
}