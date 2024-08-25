autowatch = 1;

// inlets and outlets
inlets = 1;
setinletassist(0, "bang");

outlets = 1;
setoutletassist(0, "lcd");

include("_lcd.js");

var lcd = undefined;


function bang() {

   if (!lcd)
      return;

   var commandList = lcd.renderCommands();
   for (var index in commandList) {
      var command = commandList[index];
      outlet(0, command);
   }
}

function setName(name) {

   lcd = new LCD.Device(name);

}
