const maxAPI = require('max-api');
const os = require('os');
const https = require("https");

const settings = require(os.homedir() + '/.openweathermap.json');
const api_key = settings['api_key'];

let latitude = undefined;
let longitude = undefined;
let targetDict = 'weather';

// see https://openweathermap.org/current


function outputLocation() {

   if (undefined === latitude || undefined === longitude)
      return;

   maxAPI.outlet('latitude', latitude);
   maxAPI.outlet('longitude', longitude);
}

async function updateLocation(json) {

   maxAPI.updateDict(targetDict, 'location', json);

   if (json.length > 0) {
      latitude = json[0]['lat'];
      longitude = json[0]['lon'];

      outputLocation();
   }
}

async function setViaLocation(value) {

   let locationUrl = `https://api.openweathermap.org/geo/1.0/direct?q=${value}&limit=1&appid=${api_key}`;

   const locationRequest = https.get(locationUrl, (locationResponse) => {
      console.log('statusCode:', locationResponse.statusCode);
      console.log('headers:', locationResponse.headers);

      let data = [];
      locationResponse.on('data', (chunk) => {
         data.push(chunk);
      });

      locationResponse.on('end', () => {
         try {
            const json = JSON.parse(Buffer.concat(data).toString());
            updateLocation(json);
         }
         catch (err) {
         }
      });

   });

   locationRequest.on('error', (e) => {
      console.error(e);
   });
   locationRequest.end();
}

async function updateStatus(json) {

   await maxAPI.updateDict(targetDict, 'current', json);
   maxAPI.outlet('weather', 'bang');
}


async function status() {

   let weatherUrl = `https://api.openweathermap.org/data/2.5/weather?lat=${latitude}&lon=${longitude}&units=metric&appid=${api_key}`;

   const statusRequest = https.get(weatherUrl, (statusResponse) => {
      console.log('statusCode:', statusResponse.statusCode);
      console.log('headers:', statusResponse.headers);

      let data = [];
      statusResponse.on('data', (chunk) => {
         data.push(chunk);
      });

      statusResponse.on('end', () => {
         try {
            const json = JSON.parse(Buffer.concat(data).toString());
            updateStatus(json);
         }
         catch (err) {
         }
      });

   });

   statusRequest.on('error', (e) => {
      console.error(e);
   });
   statusRequest.end();
}

maxAPI.addHandlers({
   latitude: (value) => {
      if (undefined === value)
         return;

      latitude = value;
      outputLocation();
   },
   longitude: (value) => {
      if (undefined === value)
         return;

      longitude = value;
      outputLocation();
   },
   location: async (value) => {
      if (!value)
         return;

      await setViaLocation(value);
   },
   bang: async () => {
      await status();
   },
   settarget: (value) => {

      targetDict = value;
      console.log('targetDict:', targetDict);
   }
});

