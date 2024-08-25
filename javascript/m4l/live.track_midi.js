autowatch = 1;

// inlets and outlets
inlets = 1;
setinletassist(0, "bang");

outlets = 1;
setoutletassist(0, "tracks");

function bang() {
   // createMappings();

   var api = new LiveAPI(null, "live_set");
   apiPrint(api);
}

function apiPrint(api) {

   if (api == undefined) {
      print("UNDEFINED");
      return;
   }

   for (var key in api) {
      if ("info" == key)
         continue;

      var value = api[key];
      print("KEY", key, "TYPE", typeof value, "VALUE", value);
   }

   var info = api["info"].split("\n");
   //print(typeof info);
   for (var index in info) {
      var values = info[index].split(" ");
      if (values[0] == "property") {
         var name = values[1];
         var type = values[2];
         var prop = api.get(name);
         print(name, type, prop);

      }
   }
}


createMappings.local = 1;
function createMappings() {

   print("createMappings");
   var api = new LiveAPI(null, "live_set this_device canonical_parent");

   if (!api) {
      print("no api object");
      return;
   }

   outlet(0, api.get("name"));

   apiPrint(api);
   //apiPrint(api.get("available_input_routing_channels"));


}
