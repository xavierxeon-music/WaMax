// min, max and average 

if (typeof this.MinMaxFinder === "undefined") {

   this.MinMaxFinder = class {

      constructor() {

         this.minimum = null;
         this.maximum = null;

         this.totalValue = 0.0;
         this.counter = 0;
      }

      evaluate(value) {

         if (this.minimum === null || this.minimum > value)
            this.minimum = value;

         if (this.maximum === null || this.maximum < value)
            this.maximum = value;

         this.totalValue += value;
         this.counter++;
      }

      reset() {

         this.minimum = null;
         this.maximum = null;

         this.totalValue = 0.0;
         this.counter = 0;
      }

      average() {

         if (0 === this.counter)
            return 0;
         return this.totalValue / this.counter;
      }
   }
}
