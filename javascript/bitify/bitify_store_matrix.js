autowatch = 1;

// inlets and outlets
inlets = 1;
setinletassist(0, "text");

outlets = 1;
setoutletassist(0, "text"); 

//////////////////////////////////////////

var settings = {};

function getvalueof() {
   var text = JSON.stringify(settings);
   return text;
}

function setvalueof(value) {
   settings = JSON.parse(value);
}

//////////////////////////////////////////

function bang(){
   print("bang");
}

