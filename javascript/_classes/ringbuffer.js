// ring buffer

if (typeof this.RingBuffer === "undefined") {

   this.RingBuffer = class {

      constructor(length, defaultValue = 0) {

         this.length = length;
         this.defaultValue = defaultValue;
         this.index = 0;

         this.buffer = new Array(length).fill(defaultValue);
      }

      push(content) {
         this.buffer[this.index] = content;
         this.index = (this.index + 1) % this.length;
      }

      get(index) {
         let fetch = (this.index + index) % this.length;
         return this.buffer[fetch];
      }

      clear() {
         this.index = 0;
         this.buffer = new Array(this.length).fill(this.defaultValue);
      }

      average() {
         return this.sum() / this.length;
      }

      sum() {
         let value = 0;
         for (let index = 0; index < this.length; index += 1)
            value += this.buffer[index];
         return value;
      }
   }
}