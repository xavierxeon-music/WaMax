const maxAPI = require('max-api');

const midi = require("midi");

const output = new midi.Output();

const input = new midi.Input();
input.on('message', (deltaTime, message) => {

   maxAPI.outlet(["midi", ...message]);
});


maxAPI.addHandler('open', (deviceName) => {

   for (let index = 0; index < output.getPortCount(); index++) {
      let name = output.getPortName(index);
      if (name != deviceName)
         continue;
      input.openPort(index);
      break;
   }

   for (let index = 0; index < input.getPortCount(); index++) {
      let name = input.getPortName(index);
      if (name != deviceName)
         continue;
      output.openPort(index);
      break;
   }
});


maxAPI.addHandler(maxAPI.MESSAGE_TYPES.LIST, (...message) => {
   output.sendMessage(message);
});
