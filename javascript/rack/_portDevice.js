//

PortDevice = function (name) {

   this.core = new Global(name);

   if (!this.core.portDict)
      this.core.portDict = {};

   if (!this.core.useCounter)
      this.core.useCounter = 0;

   this.receiverStart = 0;
   this.receiverLength = 0;

   this.senderStart = 0;
   this.senderLength = 0;

   return this;
}

PortDevice.prototype.setup = function (receiverStart, receiverLength, senderStart, senderLength) {

   this.receiverStart = receiverStart;
   this.receiverLength = receiverLength;

   this.senderStart = senderStart;
   this.senderLength = senderLength;
}

PortDevice.prototype.omniIndex = function (index, isSender) {

   var maxId = isSender ? this.senderLength : this.receiverLength;
   if (index <= 0 || index > maxId)
      return 0;

   var offset = isSender ? this.senderStart : this.receiverStart;
   return index + (offset - 1);
}


PortDevice.prototype.debug = function () {

   print("dict content:")
   for (var key in this.core.portDict) {
      var name = this.core.portDict[key];
      print("*", key, name);
   }
}

PortDevice.prototype.copyTo = function (targetDict) {

   targetDict.clear();

   for (var key in this.core.portDict) {
      var name = this.core.portDict[key];
      targetDict.set(key, name);
   }

}

PortDevice.prototype.register = function (updateFunction) {

   if (updateFunction)
      this.core.updateFunction = updateFunction;

   this.core.useCounter++;
   //print("reg", this.core.useCounter);
}

PortDevice.prototype.deregister = function () {

   this.core.useCounter--;
   if (0 == this.core.useCounter)
      this.core.portDict = {};

   //print("de-reg", this.core.useCounter);
}

PortDevice.prototype.addPortName = function (index, name, isSender) {

   if (!index || !name)
      return;

   var prefix = isSender ? "s" : "r";
   var key = prefix + index.toString();

   this.core.portDict[key] = name;

   //print("add", key, name); this.print();

   if (this.core.updateFunction)
      this.core.updateFunction();
}

PortDevice.prototype.removePortName = function (index, isSender) {

   if (!index)
      return;

   var prefix = isSender ? "s" : "r";
   var key = prefix + index.toString();

   delete this.core.portDict[key];
   //print("remove", key); this.print();

   if (this.core.updateFunction)
      this.core.updateFunction();
}
