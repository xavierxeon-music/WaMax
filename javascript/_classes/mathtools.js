// math

if (typeof this.MathTools === "undefined") {

   this.MathTools = class {

      static powerOfTwo(value) {

         var power = 0;
         var test = 1;

         while (test < value) {
            test *= 2;
            power += 1;
         }

         if (test === value)
            return power;
         else
            return -1;
      }

      static getByte(value, index) {

         var shift = index * 8;
         var mask = 0xff << shift;

         var extract = value & mask;
         var byte = extract >> shift;

         return byte;
      }
   }
}
