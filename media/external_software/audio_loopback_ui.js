//
setupDocument(165, 1, 1);
new Title("audio loopback");

let area = new Div(document.body);
area.setColor("#373737");
area.setGridLayout([15, 60, 15, 60]);


// content

let portDict = {};

for (let index = 1; index <= 8; index++) {

   let leftIndex = 0 + index;
   new Div(area, "<b>" + leftIndex.toString() + ":</b>", "right");
   let leftName = new Div(area, "");
   portDict[leftIndex] = leftName;

   let rightIndex = 8 + index;
   new Div(area, "<b>" + rightIndex.toString() + ":</b>", "right");
   let rightName = new Div(area, "");
   portDict[rightIndex] = rightName;
}

// functionality

max.bindInlet('loadDict', loadDict);
function loadDict() {

   max.getDict("loopback_names", function (maxDict) {

      for (id in maxDict) {

         let name = maxDict[id];
         portDict[id].setText(name);
      }
   });
}

