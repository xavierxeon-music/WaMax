//

var mixer = new Global("mixer");

mixer.soloIdList = [];
mixer.soloHandleList = [];

mixer.addHandle = function (handle) {

   if (handle in this.soloHandleList)
      return;

   this.soloHandleList.push(handle);
}

mixer.removeHandle = function (handle) {

   removeFromArray(this.soloHandleList, handle);
}

mixer.hasSolo = function () {

   return (0 != this.soloIdList.length);
}

mixer.unSolo = function () {

   this.soloIdList = [];
   this.callHandles(false);
}

mixer.unMute = function () {

   this.callHandles(true);
}

mixer.enableSolo = function (id) {

   if (id in this.soloIdList)
      return;

   this.soloIdList.push(id);

   //print("enableSolo", this.soloIdList);
   this.callHandles(false);
}

mixer.disableSolo = function (id) {

   removeFromArray(this.soloIdList, id);

   //print("disableSolo", this.soloIdList);
   this.callHandles(false);
}


mixer.callHandles = function (unmute) {

   for (var index = 0; index < this.soloHandleList.length; index++) {
      var handle = this.soloHandleList[index];
      handle(this.soloIdList, unmute);
   }
}
