//  inlets and outlets
inlets = 1;
setinletassist(0, "bang, message");

outlets = 3;
setoutletassist(0, "function (padClicked, forceValue");
setoutletassist(1, "off_list");
setoutletassist(2, "on_list");

var value = 8;

function padClicked(id) {
   if (id < 8)
      value = id;
   else
      value = 8;

   outlet(0, value);

   var offList = [];
   var onList = [];

   for (i = 1; i <= 8; i++) {
      if (i <= value)
         onList.push(i);
      else
         offList.push(i);
   }

   outlet(1, offList);
   outlet(2, onList);
}

function forceValue(value) {
   padClicked(value);
}


