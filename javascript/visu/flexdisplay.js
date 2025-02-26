autowatch = 1;

include("helper.js");

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
const token = 'asdasdjasd';

function upload() {

   if (!uploadFileName) {
      //Helper.debug("upload", "no file name");
      return;
   }

   uploadContent.setinfo(matrixId);
   uploadContent.frommatrix(matrixId);
   uploadContent.exportimage(uploadFileName, "png");

   let file = new File(uploadFileName, "read");
   if (!file.isopen) {
      //Helper.debug("upload", "file not open");
      return;
   }

   request = {};
   request["http_method"] = "post";
   request["url"] = baseUrl + "/image";
   request["multipart_form"] = { "part1": { "name": "file", "file": uploadFileName } };
   request["headers"] = ["Authorization: Bearer " + token, "Content-Type: multipart/form-data"];

   //Helper.debug("upload", uploadContent.dim, uploadFileName);

   outlet_dictionary(0, request);
}

function bind(id) {

   matrixId = id;
   //Helper.debug("bind", matrixId);
}

makeAbsPath.local = 1;
function makeAbsPath(path) {

   if (0 < path.indexOf("/"))
      return path;

   top_patcher = this.patcher;
   while (top_patcher.parentpatcher) {
      top_patcher = top_patcher.parentpatcher;
   }

   let file = new File(top_patcher.filepath)
   return file.foldername + "/" + path;
}

function setfiles(uploadPath, downloadPath) {

   uploadFileName = makeAbsPath(uploadPath);
   downloadFileName = makeAbsPath(downloadPath);

   //Helper.debug("setfiles", uploadFileName, downloadFileName);
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
      if (test == downloadFileName) {
         //Helper.debug("new image", test);
         downloadMatrix.importmovie(downloadFileName);
         outlet(1, "jit_matrix", downloadMatrix.name);
      }
   }

}

function fetch() {

   request = {};
   request["http_method"] = "get";
   request["headers"] = ["Authorization: Bearer " + token, "Content-Type: multipart/form-data"];

   if (downloadTimeStamp < timestamp) {

      downloadTimeStamp = timestamp;
      request["url"] = baseUrl + "/image";
      request["filename_out"] = downloadFileName;
      request["overwrite_output_file"] = 1;
      //Helper.debug("fetch image");
   }
   else {
      request["url"] = baseUrl + "/timestamp";
   }

   outlet_dictionary(0, request);
}
