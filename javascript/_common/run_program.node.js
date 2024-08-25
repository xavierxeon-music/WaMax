const spawn = require('child_process').spawn;
const exec = require('child_process').exec;
const maxAPI = require('max-api');
const path = require('path');


// functions

function printData(tag, data) {
   var text = `${data}`;
   var lines = text.split("\n");

   for (var index in lines) {
      line = lines[index];
      if (!line)
         continue;

      maxAPI.outlet([tag, line]);
   }
}

function executeProgram(program, appArgs) {

   var process = spawn(program, appArgs);

   process.stdout.on('data', (data) => {
      // maxAPI.post(`stdout: ${data}`);
      printData("stdout", data);
   });

   process.stderr.on('data', (data) => {
      // maxAPI.post(`stderr: ${data}`);
      printData("stderr", data);
   });

   process.on('error', (err) => {
      maxAPI.outlet(["exit", -1]);
   });

   process.on('exit', (code) => {
      maxAPI.outlet(["exit", code]);
   });
}

function isOpen(appName, fileName) {

   var filePath = path.dirname(fileName);
   filePath = cleanItem(filePath);
   var command = "lsof +D " + filePath + " | grep " + appName + " | wc -l";

   // bash -c "lsof +D /Users/waspe/Artwork/SpatialTest  2> /dev/null | grep Ableton | wc -l"
   exec(command, (error, stdout, stderr) => {

      if (error)
         maxAPI.outlet(["isopen", 0]);

      var result = stdout.trim();
      result = parseInt(result);

      maxAPI.outlet(["isopen", result]);

   });
   return false;
}

function cleanItem(item) {

   if (!item.includes(":"))
      return item;

   var content = item.split('/');
   item = "";
   for (var entry of content) {
      if ("Macintosh HD:" == entry)
         continue;
      if (entry.includes(":"))
         entry = "Volumes/" + entry.replace(":", "")
      item = item + "/" + entry
   }

   return item;

}

// handlers

const handlers = {

   checkOpen: (fileName, appName) => {
      isOpen(fileName, appName);
   },
   launch: (program, ...args) => {

      var appArgs = []
      for (const item of args)
         appArgs.push(item);

      executeProgram(program, appArgs);
   },
   python: (...args) => {

      var appArgs = ["-u"]
      for (const item of args)
         appArgs.push(item);

      executeProgram("/opt/homebrew/bin/python3", appArgs);
   },
   open: (...args) => {

      var appArgs = []
      for (var item of args) {
         item = cleanItem(item);
         appArgs.push(item);
      }

      executeProgram("/usr/bin/open", appArgs);
   }
};

maxAPI.addHandlers(handlers);

// main

var packagePath = path.resolve(path.dirname(__filename) + "/../../");
maxAPI.outlet(["packagePath", packagePath]);
