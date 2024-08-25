// 

RangedValue = function () {

   this.min = 0;
   this.max = 100;
   this.scale = 1.0;

   this.wrap = false;
   this.current = this.min;

   return this;
}

RangedValue.prototype.setMin = function (value) {

   if (value > this.max)
      return;
   else
      this.min = value;

   if (this.current < this.min)
      this.current = this.min;
}

RangedValue.prototype.setMax = function (value) {

   if (value < this.min)
      return;
   else
      this.max = value;

   if (this.current > this.max)
      this.current = max;
}

RangedValue.prototype.applyDiff = function (value) {

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

RangedValue.prototype.forceValue = function (value) {

   if (value < this.min)
      this.current = this.min;
   else if (value > this.max)
      this.current = this.max;
   else
      this.current = value;
}

RangedValue.prototype.setWrap = function (value) {

   this.wrap = value;
}

RangedValue.prototype.setScale = function (value) {

   this.scale = value;
}