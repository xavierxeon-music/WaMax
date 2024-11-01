// 

var __defined__RangedValue;
if (!__defined__RangedValue) {
   __defined__RangedValue = 1;

   class RangedValue {

      constructor() {

         this.min = 0;
         this.max = 100;
         this.scale = 1.0;

         this.wrap = false;
         this.current = this.min;
      }

      setMin(value) {

         if (value > this.max)
            return;
         else
            this.min = value;

         if (this.current < this.min)
            this.current = this.min;
      }

      setMax(value) {

         if (value < this.min)
            return;
         else
            this.max = value;

         if (this.current > this.max)
            this.current = max;
      }

      applyDiff(value) {

         this.current += value * this.scale;

         if (this.wrap) {
            var range = this.max - this.min;
            while (this.current >= this.max)
               this.current -= range;
            while (this.current < this.min)
               this.current += range;
         }
         else {
            if (this.current > this.max)
               this.current = this.max;
            else if (this.current < this.min)
               this.current = this.min;
         }
      }

      forceValue(value) {

         if (value < this.min)
            this.current = this.min;
         else if (value > this.max)
            this.current = this.max;
         else
            this.current = value;
      }

      setWrap(value) {

         this.wrap = value;
      }

      setScale(value) {

         this.scale = value;
      }
   }
}