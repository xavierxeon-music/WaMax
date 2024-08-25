// min, max and average 

function MinMaxFinder() {

   this.minimum = null;
   this.maximum = null;

   this.totalValue = 0.0;
   this.counter = 0;
}

MinMaxFinder.prototype.evaluate = function (value) {

   if (this.minimum === null || this.minimum > value)
      this.minimum = value;

   if (this.maximum === null || this.maximum < value)
      this.maximum = value;

   this.totalValue += value;
   this.counter++;
}

MinMaxFinder.prototype.reset = function () {

   this.minimum = null;
   this.maximum = null;

   this.totalValue = 0.0;
   this.counter = 0;
}

MinMaxFinder.prototype.average = function () {

   if (0 === this.counter)
      return 0;
   return this.totalValue / this.counter;
}
