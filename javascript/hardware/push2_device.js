autowatch = 1;

// see https://github.com/Ableton/push-interface/blob/master/doc/AbletonPush2MIDIDisplayInterface.asc

// inlets and outlets
inlets = 1;
setinletassist(0, "lookup");

outlets = 1;
setoutletassist(0, "id");

include("json.js")
include("color.js")
include("helper.js")

include("_push2.js");
var device = new Push2Device();

// midi routing
function midiPad(id, value) {

   var name = device.padName(id);
   device.sendCallback("pad", name, value);

   if (0 == value)
      messnamed("push2_pad_released", [id, value]);
   else
      messnamed("push2_pad_pressed", [id, value]);

}

function midiButton(id, value) {

   var name = device.buttonName(id);

   var isEncoder = (device.encoderIdList.indexOf(id) > -1);
   if (isEncoder) {
      device.sendCallback("encoder", name, value);
      if (127 == value)
         messnamed("push2_encoder_down", id);
      else
         messnamed("push2_encoder_up", id);
   }
   else {
      device.sendCallback("button", name, value);
      if (0 == value)
         messnamed("push2_button_released", id);
      else
         messnamed("push2_button_pressed", id);
   }
}

function midiSlider(value) {

   messnamed("push2_slider_value", value);
}

function padClicked(name, clicked) {

   //print("PAD CLICKED", name, clicked);

   var id = device.padId(name);

   if (clicked)
      midiPad(id, 127);
   else
      midiPad(id, 0);
}

function buttonClicked(name, clicked) {

   var id = device.buttonId(name);
   if (clicked)
      midiButton(id, 127);
   else
      midiButton(id, 0);
}

function encoderClicked(name, value) {

   var id = device.buttonId(name);
   midiButton(id, value);
}

// name conversion
function push2Pad(id) {

   var name = device.padName(id);
   if (name)
      outlet(0, ["pad", name]);
}

function pad2Push(name) {

   var id = device.padId(name);
   outlet(0, id);
}

function push2Button(id) {

   var name = device.buttonName(id);
   if (!name)
      return;

   var isColor = device.buttonIsColor(name);
   if (isColor)
      outlet(0, ["color_button", name]);
   else
      outlet(0, ["button", name]);
}

function button2Push(name) {

   var id = device.buttonId(name);
   var isColor = device.buttonIsColor(name);

   outlet(0, [id, isColor]);
}

// color functions
function padColor(isColor, id, inColor) {

   var color_index = device.colorSelect(isColor, inColor);
   outlet(0, ["pad", id, color_index]);
}

function padDummyColor(isColor, id, inColor) {

   var color_index = device.colorSelect(isColor, inColor);
   var newColor = device.colorList[color_index];

   var name = device.padName(id);
   if (name)
      outlet(0, ["setColor", name, newColor]);
}

function buttonColor(isColor, id, inColor) {

   var color_index = device.colorSelect(isColor, inColor);
   outlet(0, ["button", id, color_index]);
}

function buttonDummyColor(isColor, id, inColor) {

   var color_index = device.colorSelect(isColor, inColor);
   var newColor = device.colorList[color_index];

   var name = device.buttonName(id);
   if (name)
      outlet(0, ["setColor", name, newColor]);

}
