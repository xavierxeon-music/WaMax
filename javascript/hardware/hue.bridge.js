autowatch = 1;

// inlets and outlets
inlets = 1;
setinletassist(0, "reset/state");

outlets = 2;
setoutletassist(0, "json");
setoutletassist(1, "devices");

/////////////////////////////
// vars / setup

var hue = new Global("hue");
if (!hue.stateChange)
   hue.stateChange = {};


var stateDict = new Dict("hue_state");
var deviceMap = {};
var stackMap = {};

var sendTask = new Task(sendStack);
sendTask.interval = 100; // 10 hz update rate
sendTask.repeat();

/////////////////////////////
// hue function

// see https://developers.meethue.com/develop/hue-api-v2/api-reference/#resource_light_get
// see https://developers.meethue.com/develop/hue-api-v2/api-reference/#resource_light__id__put

hue.onOff = function (deviceName, on, duration) {

   if (!duration)
      duration = 0;

   var payload = {
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

   var color = new Color(hexColor);
   [x, y, bright] = color.toCIE();

   if (!duration)
      duration = 0;

   var payload = {
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

   var color = new Color(hexColor);
   [x, y, bright] = color.toCIE();

   if (!duration)
      duration = duration;


   var payload = {
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


   var payload = {
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

   var points = []
   for (var index = 0; index < gradientList.length; index++) {

      var color = new Color(gradientList[index]);
      [x, y, bright] = color.toCIE();

      var entry = {
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


   var payload = {
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
   for (var name in hue.stateChange) {
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

   var id = deviceMap[deviceName];
   if (!id)
      return;

   if (!stackMap[id])
      stackMap[id] = {};

   for (var key in payload) {
      stackMap[id][key] = payload[key];
   }
}

sendStack.local = 1;
function sendStack() {

   if (0 == Object.keys(stackMap).length)
      return;

   for (var id in stackMap) {
      var payload = JSON.stringify(stackMap[id]);
      payload = payload.replace('{"on":0}', '{"on":false}');
      payload = payload.replace('{"on":1}', '{"on":true}');

      outlet(0, ["send", "light", id, payload]);
   }

   stackMap = {};
}

function status(group) {

   if ("light" != group) // TODO change for sensors, etc
      return;

   var stateObject = JSON.parse(stateDict.stringify());
   var data = stateObject["light"];
   for (var index in data) {
      var device = data[index];
      var id = device["id"];
      var name = device["metadata"]["name"];
      deviceMap[name] = id;

      var callback = hue.stateChange[name];
      if (!callback)
         continue;

      var deviceState = {};
      deviceState["on"] = device["on"]["on"];
      if ("dimming" in deviceState)
         deviceState["bright"] = device["dimming"]["brightness"];

      callback(deviceState);
   }

}



