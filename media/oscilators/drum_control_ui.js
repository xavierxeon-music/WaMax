// global
const minLength = 10;
const maxLength = 500;
let sliderDict = {};
let valueDict = {}; // the main dictionary
let dictName = undefined;

let graphCategories = {};

class GraphCategory {

   constructor(minValue, maxValue, hexColor) {
      this.minValue = minValue;
      this.maxValue = maxValue;
      this.hexColor = hexColor;
   }
}

class EnvelopeCanvas extends Canvas {

   constructor() {

      super(document.body, undefined, 50);
      this.padding = 5;
   }

   update() {

      this.ctx.clearRect(0, 0, this.element.width, this.element.height);

      let fullLength = this.element.width - 2 * this.padding;
      let lengthScale = fullLength / (maxLength - minLength);

      let fullHeight = this.element.height - 2 * this.padding;
      let heightScale = fullHeight / 1.0;

      for (let key in graphCategories) {

         let category = graphCategories[key];
         let categroyScale = 1.0 / (category.maxValue - category.minValue);

         let length = valueDict[key + "_length"];
         let curve = valueDict[key + "_curve"];

         let startHeight = valueDict[key + "_start"];
         let endHeight = valueDict[key + "_end"];

         // scale and reverse
         startHeight *= categroyScale;
         endHeight *= categroyScale;

         length = (length - minLength) * lengthScale;
         startHeight = (1.0 - startHeight) * heightScale;
         endHeight = (1.0 - endHeight) * heightScale;

         this.#drawCurve(startHeight, endHeight, length, curve, category.hexColor);
      }
   }

   #drawCurve(startHeight, endHeight, length, curve, color) {

      let midScale = (curve + 100.0) / 200.0;
      let midX = 0 + midScale * length;
      let midY = endHeight + midScale * (startHeight - endHeight)

      this.ctx.beginPath();
      this.ctx.moveTo(this.padding, this.padding + startHeight);
      this.ctx.lineTo(this.padding + midX, this.padding + midY);
      this.ctx.lineTo(this.padding + length, this.padding + endHeight);
      this.ctx.lineWidth = 5;
      this.ctx.strokeStyle = color;
      this.ctx.stroke();
      this.ctx.closePath();
   }
}

// gui
function createTabWidgets() {

   let tabBar = new TabBar([50, "auto", 60], 80);

   // sound
   {
      let tabAmplitude = tabBar.addTab("Amp");
      tabAmplitude.setDefault();
      tabAmplitude.setColor("#bb0000");

      new Div(tabAmplitude, "start", "right");
      let mixSlider = new Slider(tabAmplitude, "amp_start", "%");
      mixSlider.setRange(0, 100, 2);
      initSlider(mixSlider);

      new Div(tabAmplitude, "end", "right");
      let maxSlider = new Slider(tabAmplitude, "amp_end", "%");
      initSlider(maxSlider);

      new Div(tabAmplitude, "length", "right");
      let lengthSlider = new Slider(tabAmplitude, "amp_length", "ms");
      lengthSlider.setRange(minLength, maxLength, 2.0);
      initSlider(lengthSlider);

      new Div(tabAmplitude, "curve", "right");
      let curveSlider = new Slider(tabAmplitude, "amp_curve");
      curveSlider.setRange(-100.0, 100.0, 1.0);
      curveSlider.setVavlue(0);
      initSlider(curveSlider);

      graphCategories["amp"] = new GraphCategory(0.0, 100.0, "#ff0000");
   }

   // pitch
   {
      let tabPitch = tabBar.addTab("Pitch");
      tabPitch.setColor("#0000ff");

      new Div(tabPitch, "start", "right");
      let startSlider = new Slider(tabPitch, "pitch_start", "Hz");
      startSlider.setRange(100, 1000, 2);
      initSlider(startSlider);

      new Div(tabPitch, "end", "right");
      let endSlider = new Slider(tabPitch, "pitch_end", "Hz");
      endSlider.setRange(50, 501, 2);
      initSlider(endSlider);

      new Div(tabPitch, "length", "right");
      let lengthSlider = new Slider(tabPitch, "pitch_length", "ms");
      lengthSlider.setRange(minLength, maxLength, 2.0);
      initSlider(lengthSlider);

      new Div(tabPitch, "curve", "right");
      let curveSlider = new Slider(tabPitch, "pitch_curve");
      curveSlider.setRange(-100.0, 100.0, 1.0);
      curveSlider.setVavlue(0);
      initSlider(curveSlider);

      graphCategories["pitch"] = new GraphCategory(50.0, 1000.0, "#0000ff");
   }

   // noise
   {
      let tabNoise = tabBar.addTab("Noise");
      tabNoise.setColor("#00bb00");

      new Div(tabNoise, "start", "right");
      let startSlider = new Slider(tabNoise, "noise_start", "%");
      startSlider.setRange(0, 100, 2);
      initSlider(startSlider);

      new Div(tabNoise, "end", "right");
      let endSlider = new Slider(tabNoise, "noise_end", "%");
      endSlider.setRange(0, 100, 2);
      initSlider(endSlider);

      new Div(tabNoise, "length", "right");
      let lengthSlider = new Slider(tabNoise, "noise_length", "ms");
      lengthSlider.setRange(minLength, maxLength, 2.0);
      initSlider(lengthSlider);

      new Div(tabNoise, "curve", "right");
      let curveSlider = new Slider(tabNoise, "noise_curve");
      curveSlider.setRange(-100.0, 100.0, 1.0);
      curveSlider.setVavlue(0);
      initSlider(curveSlider);

      graphCategories["noise"] = new GraphCategory(0.0, 100.0, "#00ff00");
   }

   // filter
   {
      let tabFilter = tabBar.addTab("Filter");
      tabFilter.setColor("#bbbb00");

      new Div(tabFilter, "start", "right");
      let startSlider = new Slider(tabFilter, "filter_start", "Hz");
      startSlider.setRange(300, 3000, 2);
      initSlider(startSlider);

      new Div(tabFilter, "end", "right");
      let endSlider = new Slider(tabFilter, "filter_end", "Hz");
      endSlider.setRange(300, 3000, 2);
      initSlider(endSlider);

      new Div(tabFilter, "length", "right");
      let lengthSlider = new Slider(tabFilter, "filter_length", "ms");
      lengthSlider.setRange(minLength, maxLength, 2.0);
      initSlider(lengthSlider);

      new Div(tabFilter, "curve", "right");
      let curveSlider = new Slider(tabFilter, "filter_curve");
      curveSlider.setRange(-100.0, 100.0, 1.0);
      curveSlider.setVavlue(0);
      initSlider(curveSlider);

      graphCategories["filter"] = new GraphCategory(300.0, 3000.0, "#ffff00");

   }

   // Q
   {
      let tabQ = tabBar.addTab("Q");
      tabQ.setColor("#00bbbb");

      new Div(tabQ, "start", "right");
      let startSlider = new Slider(tabQ, "q_start", "%");
      startSlider.setRange(0, 100, 2);
      initSlider(startSlider);

      new Div(tabQ, "end", "right");
      let endSlider = new Slider(tabQ, "q_end", "%");
      endSlider.setRange(0, 100, 2);
      initSlider(endSlider);

      new Div(tabQ, "length", "right");
      let lengthSlider = new Slider(tabQ, "q_length", "ms");
      lengthSlider.setRange(minLength, maxLength, 2.0);
      initSlider(lengthSlider);

      new Div(tabQ, "curve", "right");
      let curveSlider = new Slider(tabQ, "q_curve");
      curveSlider.setRange(-100.0, 100.0, 1.0);
      curveSlider.setVavlue(0);
      initSlider(curveSlider);

      graphCategories["q"] = new GraphCategory(0.0, 100.0, "#00ffff");
   }
}

// content

function initSlider(slider) {

   slider.onValueChanged(sliderChanged);
   sliderDict[slider.id] = slider;
   valueDict[slider.id] = slider.getValue();
}

function sliderChanged(key, value) {

   valueDict[key] = value;

   if (dictName) {
      max.setDict(dictName, valueDict);
      max.outlet("bang");
   }

   canvas.update();
}

max.bindInlet('load', load);
function load(maxDictName) {

   dictName = maxDictName;

   max.getDict(dictName, function (maxDict) {

      for (let key in maxDict) {
         let value = maxDict[key];
         valueDict[key] = value;

         sliderDict[key].setVavlue(value);
      }
   });

   max.outlet("bang");
   canvas.update();
}

// main
setupDocument(215, 1, 1);
new Title("drum control");

let container = new Div(document.body);
container.setAttribute("class", "tabcontent");
container.setStyle("grid-template-columns", "50px auto 60px");

let sound = new Div(container, "sine/saw", "right");
let curveSlider = new Slider(container, "sound_mix", "%");
initSlider(curveSlider);

createTabWidgets();
let canvas = new EnvelopeCanvas();

// debug arrays
assocDict("test", ".vscode/drum_map.json");
