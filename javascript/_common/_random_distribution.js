///  distributed random

function RandomDistribution() {

   var densityMap = {
      "2": 1,
      "3": 2,
      "4": 3,
      "5": 4,
      "6": 5,
      "7": 6,
      "8": 5,
      "9": 4,
      "10": 3,
      "11": 2,
      "12": 1
   };

   this.init(densityMap);
}

RandomDistribution.prototype.init = function (densityMap) {

   this.targets = [];
   for (var key in densityMap) {
      var maxCount = densityMap[key];
      var value = parseInt(key);
      for (var index = 0; index < maxCount; index++) {
         this.targets.push(value);
      }
   }

   //print(this.targets);
   this.targetCount = this.targets.length;
}

RandomDistribution.prototype.value = function () {

   var index = Math.floor(Math.random() * this.targetCount);
   var value = this.targets[index];

   return value;
}