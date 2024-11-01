//

Push2Device = function () {

   this.internal = new Global("Push2");
   if (!this.internal.callbackList)
      this.internal.callbackList = [];

   this.encoderIdList = [14, 15, 71, 72, 73, 74, 75, 76, 77, 78, 79];

   this.colorIndexBuffer = {};
   this.colorWhiteIndex = 0;

   this.whiteIndexBuffer = {};

   var data = Json.readFile("push2_data.json");
   this.colorList = data["colors"];
   this.whiteList = data["whites"];
   this.padmap = data["pads"];
   this.buttonmap = data["buttons"];

   var white = new Color("ffffff");
   this.colorWhiteIndex = this._findNearestMatchInColorList(white);
   this.colorIndexBuffer[white.hex] = this.colorWhiteIndex;

   for (var index = 0; index < this.colorList.length; index += 1) {

      var color = this.colorList[index];
      this.colorIndexBuffer[color] = index;
   }

   this.whiteIndexBuffer["000000"] = 0;
   this.whiteIndexBuffer["ffffff"] = 127;

   return this;
}

Push2Device.prototype.addCallback = function (callback) {

   this.internal.callbackList.push(callback);
}

Push2Device.prototype.removeCallback = function (callback) {

   Helper.removeFromArray(this.internal.callbackList, callback);
}

Push2Device.prototype.sendCallback = function (type, name, value) {

   for (var index in this.internal.callbackList) {
      var callback = this.internal.callbackList[index];
      callback(type, name, value);
   }
}

Push2Device.prototype.setPadColor = function (name, hxecColor) {

   var id = this.padId(name);
   messnamed("push2_pad_color", 1, id, hxecColor);
}

Push2Device.prototype.setButtonColor = function (name, hxecColor) {

   var isColor = this.buttonIsColor(name);
   if (!isColor)
      return;

   var id = this.buttonId(name);
   messnamed("push2_button_color", 1, id, hxecColor);
}

Push2Device.prototype.padId = function (name) {

   if (name in this.padmap)
      return this.padmap[name];

   return 36; // fallback to pad on bottom left   
}

Push2Device.prototype.padName = function (id) {

   for (var name in this.padmap) {
      if (id === this.padmap[name])
         return name;
   }

   return undefined;
}

Push2Device.prototype.buttonId = function (name) {

   if (name in this.buttonmap)
      return this.buttonmap[name][0];

   return 85; // fallback to play button on bottom left   
}

Push2Device.prototype.buttonName = function (id) {

   for (var name in this.buttonmap) {
      if (id === this.buttonmap[name][0])
         return name;
   }

   return undefined;
}

Push2Device.prototype.buttonIsColor = function (name) {

   if (name in this.buttonmap)
      return (1 == this.buttonmap[name][1]);

   return 1; // fallback to play button on bottom left   
}

Push2Device.prototype.colorSelect = function (isColor, inColor) {

   var color = new Color(inColor);
   if (0 === isColor)
      return this._findNearestMatchInWhiteList(color);

   return this._findNearestMatchInColorList(color);
}

Push2Device.prototype._findNearestMatchInColorList = function (color) {

   if (color.hex in this.colorIndexBuffer)
      return this.colorIndexBuffer[color.hex];

   var color_index = this.colorWhiteIndex;
   var minDistance = 0;
   for (var index = 0; index < this.colorList.length; index++) {

      const test = this.colorList[index];
      const rgb = new Color(test);

      const distance = color.distance(rgb);

      if (0 === index || distance < minDistance) {
         minDistance = distance;
         color_index = index;
      }
   }

   this.colorIndexBuffer[color.hex] = color_index;
   return color_index;
}

Push2Device.prototype._findNearestMatchInWhiteList = function (color) {

   if (color.hex in this.whiteIndexBuffer)
      return this.whiteIndexBuffer[color.hex];

   const luma = color.luma();

   var white__index = 127;
   for (var index = 1; index < whiteList.length; index++) {
      const last = this.whiteList[index - 1];
      const current = this.whiteList[index];
      if (luma > last && luma <= current) {
         white__index = index;
         break;
      }
   }

   this.whiteIndexBuffer[color.hex] = white__index;
   return white__index;
}