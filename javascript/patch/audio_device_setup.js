autowatch = 1;

include("helper.js");

// inlets and outlets
inlets = 1;
setinletassist(0, "messages");

outlets = 3;
setoutletassist(0, "input_id");
setoutletassist(1, "output_id");
setoutletassist(2, "dictionary");

let audioDeviceDict = {};

let inputList = [];
let outputList = [];

let deviceName = "";
let inputEnabled = false;
let autoStart = false;

function input() {
   setValueList(inputList, [...arguments]);
}

function output() {
   setValueList(outputList, [...arguments]);
}

setValueList.local = true;
function setValueList(valueList, data) {

   let op = data.shift();

   if ("clear" === op) {
      valueList = [];
      return;
   }
   else if ("append" === op) {
      let content = data.join(" ");
      //print("append", data);
      valueList.push(content);
      return;
   }
}

function name(text) {
   deviceName = text;
}

function input_enabled(value) {
   inputEnabled = (value != 0);
}

function auto_start(value) {
   autoStart = (value != 0);
}

function msg_list() {
   print("msg_list");
}

function setup() {

   let audioDeviceDict = compileDict();

   let inputId = 0;
   if (inputEnabled)
      inputId = indexInList(inputList, deviceName);

   let outputId = indexInList(outputList, deviceName);

   let driver = getOsDriver();
   messnamed("dsp", ["setdriver", driver]);

   outlet(0, inputId);
   outlet(1, outputId);

   messnamed("dsp", ["sr", 48000]);
   messnamed("dsp", ["iovs", 128]);

   if (autoStart)
      messnamed("dsp", ["start"]);

   audioDeviceDict["values"] = { 'input': inputId, 'output': outputId, 'driver': driver };
   outlet_dictionary(2, audioDeviceDict);
}

indexInList.local = true;
function indexInList(list, key) {

   for (let index in list) {
      index = parseInt(index);
      let value = list[index];
      if (value.includes(key)) {
         return index;
      }
   }

   return 0;
}

getOsDriver.local = true;
function getOsDriver() {

   if ("windows" === max.os)
      return "ad_mme";
   else
      return "CoreAudio";
}

compileDict.local = 1;
function compileDict() {

   let audioDeviceDict = {};
   audioDeviceDict["name"] = deviceName;
   audioDeviceDict["input_enabled"] = inputEnabled;
   audioDeviceDict["auto_start"] = autoStart;
   audioDeviceDict["input"] = inputList;
   audioDeviceDict["output"] = outputList;
   audioDeviceDict["os"] = max.os;

   return audioDeviceDict;

}

