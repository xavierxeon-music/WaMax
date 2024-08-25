autowatch = 1;

// inlets and outlets
inlets = 1;
setinletassist(0, "message");

outlets = 1;
setoutletassist(0, "done");

function compileData(size) {

   // init
   data = [];
   for (var x = 0; x < size; x++) {
      var row = [];
      for (var y = 0; y < size; y++) {
         row.push(0.0);
      }
      data.push(row);
   }

   var power = powerOfTwo(size);
   if (-1 === power)
      return;

   // fill
   var startPower = 2;
   var square = Math.pow(2, startPower);

   // for each poser
   for (var p = startPower; p < power; p++) {

      // step through data
      for (var x = 0; x < size; x += square) {
         for (var y = 0; y < size;
            y += square) {

            // fill value
            var value = Math.random() * 2;
            //value = Math.floor(value);
            for (var x2 = 0; x2 < square; x2++) {
               for (var y2 = 0; y2 < square; y2++) {
                  data[x + x2][y + y2] += value;
               }
            } // fill value
         }
      } // data

      square *= 2;
   } // power

   // normalize

   var minValue = power;
   var maxValue = 0;

   for (var x = 0; x < size; x++) {
      for (var y = 0; y < size; y++) {
         var value = data[x][y];

         if (value < minValue)
            minValue = value;
         if (value > maxValue)
            maxValue = value;
      }
   }

   var diff = (maxValue - minValue);
   print(minValue, maxValue, diff);

   for (var x = 0; x < size; x++) {
      for (var y = 0; y < size; y++) {
         var value = data[x][y] - minValue;
         value = value / diff;
         value = value - 0.5;
         data[x][y] = value;
      }
   }
   return data;
}

function create(name, size) {

   var buffer = new Buffer(name);
   buffer.send("sizeinsamps", size * size, 1);

   var data = compileData(size);

   for (var index = 0; index < size; index++) {

      var frame = index * 512;
      buffer.poke(1, frame, data[index]);
   }

   outlet(0, "bang");
}