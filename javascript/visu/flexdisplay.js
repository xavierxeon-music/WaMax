autowatch = 1;

include("helper.js");

// inlets and outlets
inlets = 1;
setinletassist(0, "upload, bind");

outlets = 1;
setoutletassist(0, "dump");

content = new JitterMatrix();

function upload() {

   let fileName = "t:/test.png";

   Helper.debug("upload", content.dim, fileName);
   content.exportimage(fileName, "png");

   let f = new File(fileName, "read");
   Helper.debug(f.isopen); // true, if myfile.txt is in the Max search path
   f.close();

   const fileContent = "Hello, this is the content of the file.";
   const file = new File([fileContent], fileName, { type: "text/plain" });
   const endpoint = "https://schweinesystem.ddns.net/flexdisplay/set_value/upload";

   let formData = new FormData();
   formData.append("file", file);
   fetch(endpoint, {
      method: "POST",
      body: formData
   });
}

function bind(matrixId) {

   content.setinfo(matrixId);
   content.frommatrix(matrixId);

   Helper.debug("bind", matrixId);
}

