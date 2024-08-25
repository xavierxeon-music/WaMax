//
setupDocument(178, 1, 1);
new Title("audio device");

let tabBar = new TabBar([70, 30, 70]);
let tabA = tabBar.addTab("ES9");
tabA.setDefault();



// content

let portDict = {};

for (let index = 1; index <= 12; index++) {

   let sendKey = "s" + index.toString();
   let sendPort = new Div(tabA, "&#9675;", "right", sendKey);
   portDict[sendKey] = sendPort;

   let center = new Div(tabA, index, "center");
   center.setStyle("font-weight", "bold");

   if (index <= 8) {
      let receiveKey = "r" + index.toString();
      let receivePort = new Div(tabA, "&#9675;", "left", receiveKey);
      portDict[receiveKey] = receivePort;
   }
   else {
      new Div(tabA, "", "left");
   }
}

// functionality

function blank() {

   for (let key in portDict) {
      let output = portDict[key];
      output.setText("&#9675;");
   }
}

max.bindInlet('loadDict', loadDict);
function loadDict() {

   blank();

   max.getDict("es9Display", function (maxDict) {

      for (key in maxDict) {

         let output = portDict[key];
         if (!output)
            continue;

         let text = maxDict[key];
         if (key[0] == "s")
            text = text + " &#10132;";
         else
            text = "&#10132; " + text;
         output.setText(text);
      }
   });
}

