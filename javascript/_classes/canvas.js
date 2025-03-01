// canvas

include("color.js");


if (typeof this.Canvas === "undefined") {

   this.Shape = class {

      constructor(color) {
         this.color = color;
      }

      draw(sketch) {
         throw new Error("Shape must implemtn draw function");
      }

      pixel(matrix, x, y, color) {
         matrix.setcell2d(x, y, color.red, color.green, color.blue);
      }
   }

   this.Line = class extends Shape {

      constructor(x1, y1, x2, y2, color) {
         super(color);
         this.x1 = Math.min(x1, x2);
         this.y1 = Math.min(y1, y2);
         this.x2 = Math.max(x1, x2);
         this.y2 = Math.max(y1, y2);

         this.slope = (this.y2 - this.y1) / (this.x2 - this.x1);
      }

      draw(matrix) {

         for (let x = this.x1; x <= this.x2; x++) {
            let y = this.y1 + this.slope * (x - this.x1);
            this.pixel(matrix, x, y, this.color);
         }

      }
   }

   this.Box = class extends Shape {

      constructor(x, y, width, height, color) {
         super(color);
         this.x = x;
         this.y = y;
         this.width = width;
         this.height = height;
      }

      draw(matrix) {
         for (let x = this.x; x < this.x + this.width; x++) {
            for (let y = this.y; y < this.y + this.height; y++) {
               this.pixel(matrix, x, y, this.color);
            }
         }
      }
   }

   this.Circle = class extends Shape {

      constructor(x, y, radius, color) {
         super(color);
         this.x = x;
         this.y = y;
         this.radius = radius;
      }

      draw(matrix) {
         for (let x = this.x - this.radius; x <= this.x + this.radius; x++) {
            for (let y = this.y - this.radius; y <= this.y + this.radius; y++) {
               if (Math.sqrt((x - this.x) ** 2 + (y - this.y) ** 2) <= this.radius) {
                  this.pixel(matrix, x, y, this.color);
               }
            }
         }
      }
   }

   this.Text = class extends Shape {

      constructor(x, y, text, color) {
         super(color);
         this.x = x;
         this.y = y;
         this.text = text;
      }

      draw(matrix) {
         matrix.glcolor(this.color.red / 255, this.color.green / 255, this.color.blue / 255, 1);
         matrix.gltext(this.text, this.x, this.y);
      }
   }

   this.Canvas = class {

      constructor(width, height, bgHexColor = "ffffff") {

         this.width = width;
         this.height = height;
         this.bgcolor = new Color(bgHexColor);

         this.matrix = new JitterMatrix(3, "char", width, height);
         this.shapeList = [];
      }

      clear() {
         this.shapeList = [];
      }

      addShape(shape) {
         this.shapeList.push(shape);
      }

      toOutlet(index) {
         this.matrix.setall(this.bgcolor.red, this.bgcolor.green, this.bgcolor.blue);
         for (let shape of this.shapeList) {
            shape.draw(this.matrix);
         }
         outlet(index, "jit_matrix", this.matrix.name);
      }
   }
}

