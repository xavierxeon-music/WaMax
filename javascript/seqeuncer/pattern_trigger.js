autowatch = 1;

// inlets and outlets
inlets = 1;
setinletassist(0, "message, bang");

outlets = 1;
setoutletassist(0, "on");

Pattern = function (data) {

   this.length = 0;
   this.loop = false;
   this.values = [];

   if (data) {
      this.length = data['length'];
      this.loop = (1 == data['loop']);

      var content = data['values'];
      for (var byteIndex in content) {
         var value = content[byteIndex];

         for (var index = 0; index < 8; index++) {
            var bitIndex = (8 * byteIndex) + index;
            var mask = (1 << index);
            var on = (0 != (value & mask));
            this.values.push(on);
         }

      }
   }

   this.current = 0;

   return this;
}

Pattern.prototype.advance = function () {

   if (!this.loop && this.current >= this.length)
      return;

   on = this.values[this.current];

   this.current += 1;
   if (this.current >= this.length) {
      if (this.loop)
         this.current = 0;
   }

   return on;
}



var patternStore = {};
var patternCurrent = {};


var timePoint = "1.1"



function bang() {

   for (var tag in patternCurrent) {
      pattern = patternCurrent[tag];
      on = pattern.advance();
      outlet(0, [tag, on]);
   }
}

function time(bar, beat) {

   var tp = bar.toString() + "." + beat.toString();
   tpData = patternStore[tp];
   if (!tpData)
      return;

   for (var tag in tpData) {
      pattern = tpData[tag];
      patternCurrent[tag] = pattern;
      // print(pattern, tag);
   }
}

function load(fileName) {

   var content = readJsonFile(fileName);
   for (var tag in content) {
      tagData = content[tag];
      for (var tp in tagData) {
         patternData = tagData[tp];
         if (!patternStore[tp])
            patternStore[tp] = {};
         patternStore[tp][tag] = new Pattern(patternData);
      }
   }
   // printObject(patternStore, "load");
}

function restart() {

   timePoint = "1.1"
   patternCurrent = {};
}
