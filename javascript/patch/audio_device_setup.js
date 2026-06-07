autowatch = 1;

include( "helper.js" );


// inlets and outlets
inlets = 1;
setinletassist( 0, "messages" );

outlets = 3;
setoutletassist( 0, "input_id" );
setoutletassist( 1, "output_id" );
setoutletassist( 2, "dictionary" );

let inputList = [];
let outputList = [];

let deviceName = "";
let inputEnabled = false;
let autoStart = false;

let audioDeviceDict = {};
audioDeviceDict[ "name" ] = deviceName;
audioDeviceDict[ "input_enabled" ] = inputEnabled;
audioDeviceDict[ "auto_start" ] = autoStart;
audioDeviceDict[ "os" ] = max.os;


// set values

function input_list()
{
   inputList = valueListOperation( inputList, [ ...arguments ] );
   audioDeviceDict[ "input" ] = inputList;
   outlet_dictionary( 2, audioDeviceDict );
}

function output_list()
{
   outputList = valueListOperation( outputList, [ ...arguments ] );
   audioDeviceDict[ "output" ] = outputList;
   outlet_dictionary( 2, audioDeviceDict );
}

function name( text )
{
   deviceName = text;
   audioDeviceDict[ "name" ] = deviceName;
   outlet_dictionary( 2, audioDeviceDict );
}

function input_enabled( value )
{
   inputEnabled = ( value != 0 );
   audioDeviceDict[ "input_enabled" ] = inputEnabled;
   outlet_dictionary( 2, audioDeviceDict );
}

function auto_start( value )
{
   autoStart = ( value != 0 );
   audioDeviceDict[ "auto_start" ] = autoStart;
   outlet_dictionary( 2, audioDeviceDict );
}

// choose the audio device and start the audio engine

function setup( homePath )
{
   audioDeviceDict[ "home_path" ] = homePath;

   let inputId = 0;
   if ( inputEnabled )
      inputId = indexInList( inputList, deviceName );

   let outputId = indexInList( outputList, deviceName );

   let driver = getOsDriver();
   messnamed( "dsp", [ "setdriver", driver ] );

   outlet( 0, inputId );
   outlet( 1, outputId );

   messnamed( "dsp", [ "sr", 48000 ] );
   messnamed( "dsp", [ "iovs", 128 ] );

   if ( autoStart )
      messnamed( "dsp", [ "start" ] );

   audioDeviceDict[ "values" ] = { 'input': inputId, 'output': outputId, 'driver': driver };
   outlet_dictionary( 2, audioDeviceDict );
}

// helper functions

valueListOperation.local = true;
function valueListOperation( valueList, data )
{
   let op = data.shift();
   if ( "clear" === op )
   {
      valueList = [];
   }
   else if ( "append" === op )
   {
      let content = data.join( " " );
      valueList.push( content );
   }

   return valueList;
}

indexInList.local = true;
function indexInList( list, key )
{

   for ( let index in list )
   {
      index = parseInt( index );
      let value = list[ index ];
      if ( value.includes( key ) )
      {
         return index;
      }
   }

   return 0;
}

getOsDriver.local = true;
function getOsDriver()
{
   if ( "windows" === max.os )
      return "ad_mme";
   else
      return "Core Audio";
}


