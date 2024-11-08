autowatch = 1;

include("color.js");
include("helper.js");

// inlets and outlets
inlets = 1;
setinletassist(0, "reset/state");

outlets = 2;
setoutletassist(0, "json");
setoutletassist(1, "devices");

/////////////////////////////
// vars / setup

let hue = new Global("hue");
if (!hue.stateChange)
   hue.stateChange = {};


let stateDict = new Dict("hue_state");
let deviceMap = {};
let stackMap = {};

let sendTask = new Task(sendStack);
sendTask.interval = 100; // 10 hz update rate
sendTask.repeat();

/////////////////////////////
// hue function

// see https://developers.meethue.com/develop/hue-api-v2/api-reference/#resource_light_get
// see https://developers.meethue.com/develop/hue-api-v2/api-reference/#resource_light__id__put

hue.onOff = function (deviceName, on, duration) {

   if (!duration)
      duration = 0;

   let payload = {
      "on": {
         "on": on
      },
      "dynamics": {
         "duration": duration
      }
   };

   addStackPaylod(deviceName, payload);
}

hue.coloronly = function (deviceName, hexColor, duration) {

   let color = new Color(hexColor);
   [x, y, bright] = color.toCIE();

   if (!duration)
      duration = 0;

   let payload = {
      "color": {
         "xy": {
            "x": x,
            "y": y
         }
      },
      "dynamics": {
         "duration": duration
      }
   };
   addStackPaylod(deviceName, payload);
}

hue.colorbright = function (deviceName, hexColor, duration) {

   let color = new Color(hexColor);
   [x, y, bright] = color.toCIE();

   if (!duration)
      duration = duration;


   let payload = {
      "dimming": {
         "brightness": Math.round(bright * 100)
      },
      "color": {
         "xy": {
            "x": x,
            "y": y
         }
      },
      "dynamics": {
         "duration": duration
      }
   };
   addStackPaylod(deviceName, payload);
}

hue.brightness = function (deviceName, value, duration) {

   if (!duration)
      duration = 0;


   let payload = {
      "dimming": {
         "brightness": value
      },
      "dynamics": {
         "duration": duration
      }
   };
   addStackPaylod(deviceName, payload);
}

hue.gradient = function (deviceName, gradientList, duration) {

   let points = []
   for (let index = 0; index < gradientList.length; index++) {

      let color = new Color(gradientList[index]);
      [x, y, bright] = color.toCIE();

      let entry = {
         "color": {
            "xy": {
               "x": x,
               "y": y
            }
         }
      }
      points.push(entry)
   }

   if (!duration)
      duration = 0;


   let payload = {
      "gradient": {
         "points": points,
      },
      "dynamics": {
         "duration": duration
      }
   };
   addStackPaylod(deviceName, payload);
}

/////////////////////////////
// function

function bang() {

   outlet(1, Object.keys(deviceMap));
   for (let name in hue.stateChange) {
      print("state change name", name);
   }
}

defaultStack.local = 1;
function defaultStack() {

   return {};
}

addStackPaylod.local = 1;
function addStackPaylod(deviceName, payload) {

   if (!deviceName)
      return;

   let id = deviceMap[deviceName];
   if (!id)
      return;

   if (!stackMap[id])
      stackMap[id] = {};

   for (let key in payload) {
      stackMap[id][key] = payload[key];
   }
}

sendStack.local = 1;
function sendStack() {

   if (0 == Object.keys(stackMap).length)
      return;

   for (let id in stackMap) {
      let payload = JSON.stringify(stackMap[id]);
      payload = payload.replace('{"on":0}', '{"on":false}');
      payload = payload.replace('{"on":1}', '{"on":true}');

      outlet(0, ["send", "light", id, payload]);
   }

   stackMap = {};
}

function status(group) {

   if ("light" != group) // TODO change for sensors, etc
      return;

   let stateObject = JSON.parse(stateDict.stringify());
   let data = stateObject["light"];
   for (let index in data) {
      let device = data[index];
      let id = device["id"];
      let name = device["metadata"]["name"];
      deviceMap[name] = id;

      let callback = hue.stateChange[name];
      if (!callback)
         continue;

      let deviceState = {};
      deviceState["on"] = device["on"]["on"];
      if ("dimming" in deviceState)
         deviceState["bright"] = device["dimming"]["brightness"];

      callback(deviceState);
   }

}



