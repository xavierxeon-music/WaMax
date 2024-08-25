// a tuning system

Tuning = function (notesPerOctave, baseFrequency) {

   this.notesPerOctave = notesPerOctave;
   this.baseFrequency = baseFrequency;

   this.frequencies = {};
   for (var index = 0; index < 1 + notesPerOctave; index++) {
      var fraction = index / notesPerOctave;
      var frequency = baseFrequency * Math.pow(2, fraction);

      this.frequencies[index] = frequency;
   }

   return this;
}

Tuning.prototype.lookupIndex = function (index) {

   if (index < 0)
      return this.frequencies[0];

   var inOctaveIndex = index % this.notesPerOctave;
   var octave = (index - inOctaveIndex) / this.notesPerOctave;

   var factor = Math.pow(2, octave);

   var frequency = this.frequencies[inOctaveIndex] * factor;

   return frequency;
}



Tuning.prototype.nearestFrequency = function (value) {

   var lower = this.frequencies[0];
   if (value < lower)
      return this.frequencies[0];

   var octave = 0;
   var upper = this.frequencies[this.notesPerOctave];

   while (value >= upper) {
      octave += 1;
      value /= 2.0;
   }

   var factor = Math.pow(2, octave);

   var fraction = Math.log(value / lower);
   fraction = fraction / Math.log(2);

   var index = fraction * this.notesPerOctave;
   index = Math.round(index);

   var frequency = this.frequencies[index] * factor;

   return frequency;
}
