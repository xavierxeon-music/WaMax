const maxAPI = require('max-api');
const os = require('os');
const https = require("https");

const settings = require(os.homedir() + '/.ApiKeys/openweathermap.json');
const api_key = settings['api_key'];

// max 
maxAPI.addHandlers({
   bang: async () => {
      maxAPI.post('netatmo');
   }
});
