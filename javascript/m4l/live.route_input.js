autowatch = 1;

// inlets and outlets
inlets = 1;
setinletassist(0, "message");

outlets = 1;
setoutletassist(0, "tracks");

var channelRouteName = "Post FX";
var inputId = 0;

var pluginId = 0;
var channelTypeDict = {};
var channelRouteDict = {};

function bang() {
   createMappings();
}

function setInputId(value) {
   inputId = value - 1;
   if (inputId < 0)
      inputId = 0;

   print("setInputId", inputId);
}

function setChannel(name) {

   var index = channelTypeDict[name];
   if (index == undefined) {
      // apiPrint(channelTypeDict);
      return;
   }

   print("setChannel", name, index);
   var typeString = JSON.stringify({ "identifier": index });
   var routeString = JSON.stringify({ "identifier": channelRouteDict[channelRouteName] });

   print(typeString, routeString);

   var api = new LiveAPI(null, "id " + pluginId);

   if (api == undefined) {
      print("UNDEFINED");
      return;
   }

   api.set("routing_type", typeString);
   api.set("routing_channel", routeString);

}

createMappings.local = 1;
function createMappings() {

   print("createMappings");
   if (inputId == undefined)
      return;

   var api = new LiveAPI(null, "live_set this_device");

   if (!api) {
      print("no api object");
      return;
   }

   var inputCount = api.getcount("audio_inputs");
   if (inputCount <= inputId)
      return;

   var inputPath = "live_set this_device audio_inputs " + inputId
   api.path = inputPath;
   pluginId = api.id;

   print("plugin", pluginId);

   channelTypeDict = dictParse(api, "available_routing_types");
   channelRouteDict = dictParse(api, "available_routing_channels");

   var trackNames = Object.keys(channelTypeDict);
   outlet(0, trackNames);
}
