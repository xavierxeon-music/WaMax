// max for live helpoer

var __defined__M4L;
if (!__defined__M4L) {
   __defined__M4L = 1;

   class M4l {

      static apiPrint(api) {

         if (api == undefined) {
            print("UNDEFINED");
            return;
         }

         for (var key in api) {
            // if ("info" == key)
            //    continue;
            var value = api[key];
            print("KEY", key,
               "T", typeof value,
               "V", value);
         }
      }

      static dictParse(api, key) {

         var target = {};

         var channelDict = JSON.parse(api.get(key)[0]);
         var channelList = channelDict[key];

         var channelCount = api.getcount(key);
         for (var channelCounter = 0; channelCounter < channelCount; channelCounter++) {
            var channelObject = channelList[channelCounter];
            var name = channelObject["display_name"];
            var id = channelObject["identifier"];
            target[name] = id;
         }

         return target;
      }
   }
}