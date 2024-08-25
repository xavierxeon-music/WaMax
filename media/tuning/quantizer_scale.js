
// 

let settingsName = undefined;
let settings = undefined;

const noteNames = ["note_c", "note_cs", "note_d", "note_ds", "note_e", "note_f", "note_fs", "note_g", "note_gs", "note_a", "note_as", "note_b"];
let noteActive = [false, false, false, false, false, false, false, false, false, false, false, false]

let scaleIndex = -1;

function sectionClicked(section) {

   scaleIndex = section;

   canvas.update();
   saveDict();
}

function noteClicked(note) {

   if (-1 != scaleIndex)
      return;

   let index = noteNames.indexOf(note);
   noteActive[index] = noteActive[index] ? false : true;

   canvas.update();
   saveDict();
}

//

class ClickBox {

   constructor(x, y, width, height, layer) {

      this.x = x;
      this.y = y;
      this.width = width;
      this.height = height;

      this.layer = layer;
   }

   contains(x, y) {

      if (x < this.x)
         return false;
      if (x > this.x + this.width)
         return false;

      if (y < this.y)
         return false;
      if (y > this.y + this.height)
         return false;

      return true;
   }
}

class CircleOfFiths extends Canvas {

   constructor(parent) {

      super(parent, 180, 310);

      this.img = new Image();
      this.img.src = "./tuning/CircleOfFiths.svg";

      this.clickBoxes = {};
      this.element.addEventListener("pointerdown", (clickEvent) => {
         this.#clicked(Math.round(clickEvent.offsetX), Math.round(clickEvent.offsetY));
      });

      // keys
      const kx = 10;
      const ky = 30;
      const keyWidth = 22;
      const keyHeight = 60;

      let noteNames = ["note_c", "note_d", "note_e", "note_f", "note_g", "note_a", "note_b"];
      for (let index = 0; index < 7; index++) {
         let name = noteNames[index];

         let wx = kx + (index * keyWidth);
         this.clickBoxes[name] = new ClickBox(wx, ky, keyWidth, keyHeight, 0);

         if (index == 2 || index == 6)
            continue;

         let bx = kx + 2 + ((index + 0.5) * keyWidth);
         this.clickBoxes[name + "s"] = new ClickBox(bx, ky, keyWidth - 4, 0.7 * keyHeight, 1);
      }

      // scale
      this.pieSize = (360 / 12);
      this.pieOffset = 8.5 * this.pieSize;

      this.cx = 90;
      this.cy = 190;
      this.scaleRadius = 90;
      this.clickBoxes["scale"] = new ClickBox(0, this.cy - this.scaleRadius, 2 * this.scaleRadius, 2 * this.scaleRadius, 0);

      // finalise
      this.img.onload = () => {
         this.update();
      }
   }

   update() {

      this.clear();

      this.#drawKeys(0);
      this.#drawKeys(1);

      if (scaleIndex < 0)
         this.circle(this.cx, this.cy, 25, "#cccccc");
      else
         this.#drawMarker(scaleIndex);

      this.ctx.drawImage(this.img, 0, this.cy - this.scaleRadius, 2 * this.scaleRadius, 2 * this.scaleRadius);

      this.#drawText();
   }


   #clicked(x, y) {

      let hitbox = undefined;
      let hitkey = undefined;

      for (let key in this.clickBoxes) {
         let cbox = this.clickBoxes[key];
         if (!cbox.contains(x, y))
            continue;

         if (!hitbox || hitbox.layer < cbox.layer) {
            hitkey = key;
            hitbox = cbox;
         }
      }

      if (!hitkey)
         return;

      if (hitkey.startsWith("note_"))
         noteClicked(hitkey);
      else if ("scale" == hitkey) {
         let cx = x - hitbox.x - this.scaleRadius;
         let cy = y - hitbox.y - this.scaleRadius;
         let radius = Math.sqrt(cx * cx + cy * cy);
         if (radius < 25) {
            sectionClicked(-1);
         }
         else {
            let angle = Math.atan2(cy, cx) * (180 / Math.PI);
            let sector = Math.round(angle / this.pieSize) + 3;
            if (sector < 0)
               sector += 12;
            sectionClicked(sector);
         }
      }
   }

   #polar(radius, angle) {

      let rad = angle * (Math.PI / 180);
      let x = radius * Math.cos(rad);
      let y = radius * Math.sin(rad);

      return [x, y];
   }

   #drawText() {
      this.ctx.fillStyle = "#444444";
      this.ctx.font = "bold 14px Arial";
      this.ctx.textAlign = "left";

      this.ctx.fillText("NAME", 5, 20);
      this.ctx.fillText("VISU", 10, 20 + this.cy + this.scaleRadius);

      this.ctx.fillStyle = "#7777aa";
      this.ctx.font = "bold 14px Arial";
      this.ctx.textAlign = "center";

      this.ctx.fillText("cus", this.cx, this.cy);
      this.ctx.fillText("tom", this.cx, this.cy + 10);

   }

   #drawMarker(sector) {

      let startAngle = this.pieOffset + (sector + 0) * this.pieSize;
      let endAngle = this.pieOffset + (sector + 1) * this.pieSize;

      this.ctx.fillStyle = "#cccccc";

      this.ctx.beginPath();

      let [x0, y0] = this.#polar(30, startAngle);
      this.ctx.moveTo(this.cx + x0, this.cy + y0);

      let diffAngle = endAngle - startAngle;
      for (let fraction = 0.0; fraction < 1.0; fraction += 0.1) {
         let [x1, y1] = this.#polar(this.scaleRadius, startAngle + fraction * diffAngle);
         this.ctx.lineTo(this.cx + x1, this.cy + y1);
      }

      let [x2, y2] = this.#polar(this.scaleRadius, endAngle);
      this.ctx.lineTo(this.cx + x2, this.cy + y2);

      let [x3, y3] = this.#polar(30, endAngle);
      this.ctx.lineTo(this.cx + x3, this.cy + y3);

      this.ctx.lineTo(this.cx + x0, this.cy + y0);
      this.ctx.fill();

      this.ctx.closePath();
   }

   #drawKeys(targetLayer) {

      for (let key in this.clickBoxes) {
         if (!key.startsWith("note_"))
            continue;

         let index = noteNames.indexOf(key);
         let active = noteActive[index];

         let cbox = this.clickBoxes[key];
         if (cbox.layer != targetLayer)
            continue;

         let color = undefined;
         if (1 == cbox.layer)
            color = active ? "#444444" : "#888888";
         else
            color = active ? "#eeeeee" : "#bbbbbb";

         this.box(cbox.x, cbox.y, cbox.width, cbox.height, color, "#222222");
      }
   }
};

//
setupDocument(192, 1, 1);
let title = new Title("MIDI Quantizer");

let main = new Div();
main.setStyle("background", "#444444");
main.setStyle("padding", "5px");

let canvas = new CircleOfFiths(main);
canvas.update();

let nameEdit = new TextEdit(main);
nameEdit.setStyle("width", "100px");
nameEdit.onChange(nameChanged);
nameEdit.move(60, 28);

let clearButton = new Button(main, "clear");
clearButton.onClicked(clearVisu);
clearButton.move(60, 310);

let resendButton = new Button(main, "resend");
resendButton.onClicked(resendVisu);
resendButton.move(110, 310);

// name

function nameChanged(name) {

   settings["name"] = name;
   saveDict();

}

// visu

function clearVisu() {

   max.outlet("visu", "clear");
}

function resendVisu() {

   max.outlet("visu", "resend");
}

// dict

max.bindInlet('loadDict', loadDict);
function loadDict(name) {

   // debug("LOAD DICT UI", name);
   settingsName = name;
   max.getDict(settingsName, function (maxDict) {

      settings = maxDict;

      nameEdit.setText(settings["name"]);
      noteActive = settings["noteActive"];
      scaleIndex = settings["scale"];

      canvas.update();
   });
}

function saveDict() {

   if (!settings)
      return;

   settings["noteActive"] = noteActive;
   settings["scale"] = scaleIndex;

   max.setDict(settingsName, settings);
   max.outlet("updateDict");
}
