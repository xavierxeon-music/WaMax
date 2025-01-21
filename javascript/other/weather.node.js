const maxAPI = require('max-api');
const os = require('os');
const https = require("https");

const settings = require(os.homedir() + '/.ApiKeys/openweathermap.json');
const api_key = settings['api_key'];
let be_verbose = false;

let latitude = undefined;
let longitude = undefined;
let targetDict = 'weather';

// see https://openweathermap.org/current

async function callWeatherApi(url, callback) {

   const request = https.get(url, (response) => {
      let statusCode = response.statusCode;
      if (be_verbose)
         maxAPI.post('weather:', url, statusCode, maxAPI.POST_LEVELS.INFO);

      if (200 !== statusCode) {
         if (!be_verbose)
            maxAPI.post('weather:', url, statusCode, maxAPI.POST_LEVELS.ERROR);
         maxAPI.post('headers:', response.headers, maxAPI.POST_LEVELS.ERROR);
      }

      let data = [];
      response.on('data', (chunk) => {
         data.push(chunk);
      });

      response.on('end', async () => {
         const json = JSON.parse(Buffer.concat(data).toString());
         callback(json);
      });

   });

   request.on('error', (err) => {
      console.error(err);
   });
   request.end();
}

// location
function outputLocation() {

   if (undefined === latitude || undefined === longitude)
      return;

   maxAPI.outlet('latitude', latitude);
   maxAPI.outlet('longitude', longitude);
}


// max 
maxAPI.addHandlers({
   settarget: (value) => {
      targetDict = value;
   },
   verbose: (value) => {
      be_verbose = (value == '1');
   },
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

      let locationUrl = `https://api.openweathermap.org/geo/1.0/direct?q=${value}&limit=1&appid=${api_key}`;
      await callWeatherApi(locationUrl, async (json) => {

         if (be_verbose)
            maxAPI.post('location:', locationUrl, json, maxAPI.POST_LEVELS.INFO);


         maxAPI.setDict(targetDict, { 'location': json });
         maxAPI.outlet('update', 'bang');

         if (json.length > 0) {
            latitude = json[0]['lat'];
            longitude = json[0]['lon'];

            outputLocation();
         }
      });
   },
   bang: async () => {

      let weatherUrl = `https://api.openweathermap.org/data/2.5/weather?lat=${latitude}&lon=${longitude}&units=metric&appid=${api_key}`;
      await callWeatherApi(weatherUrl, async (json) => {

         if (be_verbose)
            maxAPI.post('weather:', weatherUrl, json, maxAPI.POST_LEVELS.INFO);

         maxAPI.updateDict(targetDict, 'current', json);
         maxAPI.outlet('update', 'bang');
      });
   },
   forecast: async () => {

      let forecastUrl = `https://api.openweathermap.org/data/2.5/forecast?lat=${latitude}&lon=${longitude}&units=metric&appid=${api_key}`;
      await callWeatherApi(forecastUrl, async (json) => {

         if (be_verbose)
            maxAPI.post('forecast:', forecastUrl, json, maxAPI.POST_LEVELS.INFO);

         maxAPI.updateDict(targetDict, 'forecast', json);
         maxAPI.outlet('update', 'bang');
      });
   }
});

