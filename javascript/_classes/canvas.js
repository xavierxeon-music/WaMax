// canvas

include("color.js");


if (typeof this.Canvas === "undefined") {

   this.Canvas = class {

      constructor(width, height) {

         this.width = width;
         this.height = height;
      }

      clear() {
      }

      circle(x, y, radius, color, length, offset) {

         if (undefined == length)
            length = 1.0;
         if (undefined == offset)
            offset = 0;

         let startAngle = offset;
         let endAngle = offset + (length * 2 * Math.PI);
      }

      ring(x, y, radius, width, color, length, offset) {

         if (undefined == length)
            length = 1.0;
         if (undefined == offset)
            offset = 0;

         let startAngle = offset;
         let endAngle = offset + (length * 2 * Math.PI);
      }

      box(x, y, width, height, color, borderColor, lineWidth) {
      }

      line(x1, y1, x2, y2, color) {
      }
   }
}