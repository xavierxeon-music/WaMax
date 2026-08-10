autowatch = 1;
inlets = 1;
outlets = 1;

var TAG_REGEX = /\[id:([a-f0-9-]{36})\]/i;
var trackIndex = null;
var lastClipId = null;
var arrangementPollTask = null;

// ---------- 1. Find the track number for this device ----------
function init()
{
   var deviceApi = new LiveAPI( "this_device" );
   var path = deviceApi.unquotedpath; // e.g. "live_set tracks 2 devices 0"
   var match = /live_set tracks (\d+)/.exec( path );

   if ( !match )
   {
      post( "could not resolve track index for this device\n" );
      return;
   }
   trackIndex = parseInt( match[ 1 ] );
   post( "device is on track " + trackIndex + "\n" );

   setupWatchers();
}

// ---------- 2. Watch for the currently playing clip (session or arrangement) ----------
function setupWatchers()
{
   // Session: fires when a clip is launched/stopped on this track
   var sessionWatcher = new LiveAPI( onSessionSlotChanged, "live_set tracks " + trackIndex );
   sessionWatcher.property = "playing_slot_index";

   // Arrangement: no direct "clip changed" event, so poll playhead position while playing
   arrangementPollTask = new Task( pollArrangementPosition, this );
   arrangementPollTask.interval = 100; // ms; adjust for responsiveness vs. CPU
   arrangementPollTask.repeat();
}

function onSessionSlotChanged()
{
   var track = new LiveAPI( "live_set tracks " + trackIndex );
   var slotIdx = track.get( "playing_slot_index" )[ 0 ];

   if ( slotIdx >= 0 )
   {
      var clip = new LiveAPI( "live_set tracks " + trackIndex + " clip_slots " + slotIdx + " clip" );
      if ( clip.id && clip.id != "0" )
      {
         handlePlayingClip( clip );
      }
   }
   // slotIdx < 0 means nothing playing in session on this track;
   // arrangement polling (below) will pick up if arrangement is active instead
}

function pollArrangementPosition()
{
   var songApi = new LiveAPI( "live_set" );
   var isPlaying = songApi.get( "is_playing" )[ 0 ];
   if ( !isPlaying ) return;

   var track = new LiveAPI( "live_set tracks " + trackIndex );
   var backToArranger = track.get( "back_to_arranger" )[ 0 ];
   if ( backToArranger != 0 ) return; // session clip is overriding arrangement right now

   var currentTime = songApi.get( "current_song_time" )[ 0 ];
   var clip = findArrangementClipAtTime( trackIndex, currentTime );
   if ( clip )
   {
      handlePlayingClip( clip );
   }
}

function findArrangementClipAtTime( trackIdx, time )
{
   var track = new LiveAPI( "live_set tracks " + trackIdx );
   var clipIds = track.get( "arrangement_clips" ); // flat: ["id","123","id","456",...]

   for ( var i = 0; i < clipIds.length; i += 2 )
   {
      var clip = new LiveAPI( "id " + clipIds[ i + 1 ] );
      var start = clip.get( "start_time" )[ 0 ];
      var end = clip.get( "end_time" )[ 0 ];
      if ( time >= start && time < end )
      {
         return clip;
      }
   }
   return null;
}

// ---------- 3+4. Check for uuid in name, append if missing ----------
function handlePlayingClip( clip )
{
   if ( clip.id === lastClipId ) return; // no change, do nothing

   var currentName = clip.get( "name" )[ 0 ];
   var uuid = extractUUID( currentName );

   if ( !uuid )
   {
      uuid = generateUUID();
      clip.set( "name", currentName + " [id:" + uuid + "]" );
      post( "tagged clip with new id: " + uuid + "\n" );
   }

   lastClipId = clip.id;

   // ---------- 5. Notify on change ----------
   changePreset( uuid );
}

function extractUUID( name )
{
   var match = TAG_REGEX.exec( name );
   return match ? match[ 1 ] : null;
}

function generateUUID()
{
   return 'xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx'.replace( /[xy]/g, function ( c )
   {
      var r = Math.random() * 16 | 0;
      var v = ( c == 'x' ) ? r : ( r & 0x3 | 0x8 );
      return v.toString( 16 );
   } );
}

// ---------- stub: implement later ----------
function changePreset( uuid )
{
   post( "changePreset called with uuid: " + uuid + "\n" );
   // e.g. outlet(0, "recall", uuid);
}

init();