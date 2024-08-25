//

var LCD = {}

//////////////////////////////////////////

LCD.Device = function (deviceName) {

   this.internal = new Global("lcd");
   if (!this.internal.screen)
      this.internal.screen = {};

   this.deviceName = deviceName;

   return this;
}


LCD.Device.prototype.setScreen = function (screen) {

   this.internal.screen[this.deviceName] = screen;
}

LCD.Device.prototype.renderCommands = function () {

   var commandList = [];
   var screen = this.internal.screen[this.deviceName];
   if (!screen)
      return commandList;

   commandList.push(["dim", screen.width, screen.height]);
   commandList.push(["brgb", screen.bgColor.red, screen.bgColor.green, screen.bgColor.blue]);
   commandList.push(["clear"]);

   for (var index in screen.objectList) {
      var object = screen.objectList[index];

      var commands = object.renderCommands();
      if (Array.isArray(commands[0])) {
         for (var index2 in commands) {
            var command2 = commands[index2];
            commandList.push(command2);
         }
      }
      else {
         commandList.push(commands);
      }
   }

   return commandList;
}


//////////////////////////////////////////

LCD.Screen = function (width, height) {

   this.width = width;
   this.height = height;

   this.bgColor = new Color("black");
   this.objectList = [];

   return this;
}


LCD.Screen.prototype.clear = function () {

   this.objectList.clear();
}

LCD.Screen.prototype.addObject = function (object) {

   this.objectList.push(object);
}

LCD.Screen.prototype.removeObject = function (object) {

   removeFromArray(this.objectList, callback);
}

LCD.Screen.prototype.setBgColor = function (hexColor) {

   this.bgColor = new Color(hexColor);
}


//////////////////////////////////////////

LCD.Box = function (x, y, width, height, hexColor, fill) {

   this.x = x;
   this.y = y;
   this.width = width;
   this.height = height;

   this.penSize = 1;

   this.setColor(hexColor, fill);

   return this;
}

LCD.Box.prototype.setColor = function (hexColor, fill) {

   this.color = new Color(hexColor);

   if (undefined == fill)
      fill = true;
   this.fill = fill;
}

LCD.Box.prototype.renderCommands = function () {

   var commandList = [];
   commandList.push(["frgb", this.color.red, this.color.green, this.color.blue]);
   commandList.push(["pensize", this.penSize, this.penSize]);

   if (this.fill)
      commandList.push(["paintrect", this.x, this.y, this.x + this.width, this.y + this.height]);
   else
      commandList.push(["framerect", this.x, this.y, this.x + this.width, this.y + this.height]);

   return commandList;
}

//////////////////////////////////////////

LCD.Pie = function (x, y, width, height, hexColor, fill) {

   this.x = x;
   this.y = y;
   this.width = width;
   this.height = height;

   this.penSize = 1;

   this.startAngle = 0;
   this.arcLength = 360;

   this.setColor(hexColor, fill);

   return this;
}

LCD.Pie.prototype.setColor = function (hexColor, fill) {

   this.color = new Color(hexColor);

   if (undefined == fill)
      fill = true;

   this.fill = fill;
}

LCD.Pie.prototype.renderCommands = function () {

   var commandList = [];
   commandList.push(["frgb", this.color.red, this.color.green, this.color.blue]);
   commandList.push(["pensize", this.penSize, this.penSize]);

   if (this.fill)
      commandList.push(["paintarc", this.x, this.y, this.x + this.width, this.y + this.height, this.startAngle, this.arcLength]);
   else
      commandList.push(["framearc", this.x, this.y, this.x + this.width, this.y + this.height, this.startAngle, this.arcLength]);

   return commandList;
}

//////////////////////////////////////////

LCD.Text = function (x, y, text, hexColor, fontSize) {

   this.x = x;
   this.y = y;

   this.setColor(hexColor);
   this.setText(text, fontSize);

   return this;
}

LCD.Text.prototype.setColor = function (hexColor) {

   this.color = new Color(hexColor);
}

LCD.Text.prototype.setText = function (text, fontSize) {

   this.text = text;

   if (!fontSize)
      this.fontSize = 12;
   else
      this.fontSize = fontSize;
}

LCD.Text.prototype.renderCommands = function () {

   var commandList = [];
   commandList.push(["frgb", this.color.red, this.color.green, this.color.blue]);
   commandList.push(["moveto", this.x, this.y]);
   commandList.push(["font", "Arial", this.fontSize]);

   commandList.push(["write", this.text]);

   return commandList;
}

//////////////////////////////////////////

LCD.Line = function (x1, y1, x2, y2, hexColor) {

   this.x1 = x1;
   this.y1 = y1;

   this.x2 = x2;
   this.y2 = y2;

   this.setColor(hexColor);

   return this;
}

LCD.Line.prototype.setColor = function (hexColor) {

   this.color = new Color(hexColor);
}

LCD.Line.prototype.renderCommands = function () {

   var commandList = [];
   commandList.push(["frgb", this.color.red, this.color.green, this.color.blue]);
   commandList.push(["moveto", this.x, this.y]);
   commandList.push(["font", "Arial", this.fontSize]);

   commandList.push(["write", this.text]);

   return commandList;
}

