autowatch = 1;

include("helper.js");
include("paths.js");
include("json.js");

// inlets and outlets
inlets = 1;
setinletassist(0, "upload, bind, fetch");

outlets = 2;
setoutletassist(0, "matrix");
setoutletassist(1, "curl");

let uploadContent = new JitterMatrix();
let uploadFileName = null;
let matrixId = null;

let downloadFileName = null;
let timestamp = 0;
let downloadTimeStamp = 0;
let downloadMatrix = new JitterMatrix(4, "char", 320, 240);
downloadMatrix.adapt = 1;

const baseUrl = "https://schweinesystem.ddns.net/flexdisplay";
//const baseUrl = "http://127.0.0.1:5000";
let token = null;

function upload() {

   if (!uploadFileName) {
      print("upload", "no file name");
      return;
   }

   uploadContent.setinfo(matrixId);
   uploadContent.frommatrix(matrixId);
   uploadContent.exportimage(uploadFileName, "png");

   if (!Paths.exists(uploadFileName)) {
      print("upload", "file not open");
      return;
   }

   if (!token) {
      print("upload", "no token");
      return;
   }

   request = {};
   request["http_method"] = "post";
   request["url"] = baseUrl + "/image";
   request["multipart_form"] = { "part1": { "name": "file", "file": uploadFileName } };
   request["headers"] = ["Authorization: Bearer " + token, "Content-Type: multipart/form-data"];

   //print("upload", uploadContent.dim, uploadFileName);

   outlet_dictionary(0, request);
}

function bind(id) {

   matrixId = id;
   timestamp = 0;
   //print("bind", matrixId);
}

makeAbsPath.local = 1;
function makeAbsPath(path) {

   if (0 < path.indexOf("/"))
      return path;

   return Paths.topPatcher(this.patcher) + "/" + path;
}

function setfiles(uploadPath, downloadPath) {

   uploadFileName = makeAbsPath(uploadPath);
   downloadFileName = makeAbsPath(downloadPath);

   //print("setfiles", uploadFileName, downloadFileName);
}

function msg_dictionary(data) {

   if (200 != data["status"])
      return;

   if (data["url"].includes("timestamp")) {
      const body = JSON.parse(data["body"]);
      let newTimeStamp = body["timestamp"];
      if (newTimeStamp != timestamp) {
         timestamp = newTimeStamp;
      }
   }
   else if (data["url"].includes("image")) {
      let test = data["filename_out"];
      if (!test)
         return;

      test = test.replaceAll("\\", "/");
      if (test == downloadFileName) {
         //print("new image", test);
         downloadMatrix.importmovie(downloadFileName);
         outlet(1, "jit_matrix", downloadMatrix.name);
      }
   }
}

function fetch() {

   if (!token) {
      print("fetch", "no token");
      return;
   }

   request = {};
   request["http_method"] = "get";
   request["headers"] = ["Authorization: Bearer " + token, "Content-Type: multipart/form-data"];

   if (downloadTimeStamp < timestamp) {

      downloadTimeStamp = timestamp;
      request["url"] = baseUrl + "/image";
      request["filename_out"] = downloadFileName;
      request["overwrite_output_file"] = 1;
      //print("fetch image");
   }
   else {
      request["url"] = baseUrl + "/timestamp";
   }

   outlet_dictionary(0, request);
}

function setToken(newToken) {

   token = newToken;
} 
