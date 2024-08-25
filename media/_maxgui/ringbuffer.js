// ring buffer

class RingBuffer {

   constructor(length, defaultValue) {

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
}