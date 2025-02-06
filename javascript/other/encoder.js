autowatch = 1;

include("ranged_value.js");

//  inlets and outlets
inlets = 1;
setinletassist(0, "message");

outlets = 1;
setoutletassist(0, "value");

//////////////////////////////////////////

var rangedValue = new RangedValue();

function getvalueof() {
   return rangedValue.current
}

function setvalueof(value) {
   rangedValue.forceValue(value)
   outlet(0, rangedValue.current);
}

//////////////////////////////////////////


function setMin(value) {

   rangedValue.setMin(value);
   outlet(0, rangedValue.current);
}

function setMax(value) {

   rangedValue.setMax(value);
   outlet(0, rangedValue.current);
}

function diff(value) {

   rangedValue.applyDiff(value);
   outlet(0, rangedValue.current);
}

function forceValue(value) {

   rangedValue.forceValue(value);
   outlet(0, rangedValue.current);
}

function wrap(value) {

   rangedValue.setWrap(value);
}

function delta(value) {

   rangedValue.setScale(value);
}


