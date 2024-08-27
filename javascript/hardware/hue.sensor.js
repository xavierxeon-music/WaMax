autowatch = 1;

// inlets and outlets
inlets = 2;
setinletassist(0, "message");
setinletassist(1, "response");

outlets = 2;
setoutletassist(0, "state");
setoutletassist(1, "request");


var baseUrl = undefined;
var deviceId = undefined;

var sendQueue = {};
var sendTask = new Task(processQueue);
sendTask.intervall = 10;
sendTask.repeat();

var stateTask = new Task(state);
stateTask.intervall = 500;
stateTask.repeat();

// see https://kinsta.com/knowledgebase/javascript-http-request/


function init(deviceName, settingsFileName) {

   var settings = Json.readFile(settingsFileName);
   baseUrl = "http://" + settings["bridge"] + "/api/" + settings["username"] + "/";

   var initResponse = function () {

      if (request.status != 200)
         return;

      var response = JSON.parse(request.responseText);
      for (var key in response) {
         var device = response[key];
         if (device["name"] == deviceName)
            deviceId = key;
      }
   }

   var request = new XMLHttpRequest();
   request.open("GET", baseUrl + 'lights');
   request.onreadystatechange = initResponse;
   request.send();
}

state.local = 1;
function state() {

   if (!deviceId)
      return;

   var request = new XMLHttpRequest();

   var stateResponse = function () {

      if (request.status != 200)
         return;

      var response = JSON.parse(request.responseText);
      var on = response["state"]["on"];
      outlet(0, on);
   }

   request.open("GET", baseUrl + 'lights/' + deviceId);
   request.onreadystatechange = stateResponse;
   request.send();
}

sendToBridge.local = 1;
function sendToBridge(payload) {

   if (!deviceId)
      return;

   var request = new XMLHttpRequest();

   var sendResponse = function () {
      if (request.status == 200)
         return;

      // print(request.responseText);
   }

   request.open("PUT", baseUrl + 'lights/' + deviceId + '/state');
   request.onreadystatechange = sendResponse;

   var content = JSON.stringify(payload);
   // print(content)
   request.send(content);
}

addToQueue.local = 1;
function processQueue() {

   for (var key in sendQueue) {
      payload = sendQueue[key];
      if (!payload)
         continue;

      sendQueue[key] = undefined;
      sendToBridge(payload);
   }

}

addToQueue.local = 1;
function addToQueue(key, paylod) {

   sendQueue[key] = paylod;
}

function on() {

   var payload = { "on": true };
   addToQueue('state', payload);
}

function off() {

   var payload = { "on": false };
   addToQueue('stae', payload);
}

function color(hexColor) {

   var color = new Color(hexColor);

   [hue, sat, bright] = color.toHSV();
   hue *= 256;

   var payload = { "hue": hue, "sat": sat, "transitiontime": 0, "alert": "none" };
   addToQueue('color', payload);
}

function colorbright(hexColor) {

   var color = new Color(hexColor);

   [hue, sat, bright] = color.toHSV();
   hue *= 256;

   var payload = { "hue": hue, "sat": sat, "bri": bright, "transitiontime": 0, "alert": "none" };
   addToQueue('colorbright', payload);
}

function brightness(value) {

   var payload = { "bri": value, "transitiontime": 0, "alert": "none" };
   addToQueue('bright', payload);
}
