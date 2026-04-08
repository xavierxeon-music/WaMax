autowatch = 1;

include( "helper.js" );

// inlets and outlets
inlets = 1;
setinletassist( 0, "bang, note" );

outlets = 2;
setoutletassist( 0, "frequency" );
setoutletassist( 1, "name" );

//---------------------------------

// see https://adammurray.link/max-for-live/v8-in-live/live-api/
// see https://docs.cycling74.com/apiref/lom/tuningsystem/

class Pitch
{
   constructor ( text )
   {
      let json = JSON.parse( text );
      this.indexInOctave = json.index_in_octave;
      this.octave = json.octave;
      this.frequency = json.frequency;
   }
}

class TuningSystem
{
   constructor ()
   {
      this.tuningObject = null;
      this.frequencies = null;

      this.refFrequency = null;
      this.refNote = null;
   }

   valid()
   {
      return ( this.tuningObject != null );
   }

   read()
   {
      this.tuningObject = new LiveAPI( null, "live_set tuning_system" );
      if ( this.tuningObject == null || this.tuningObject == undefined )
      {
         print( "no tuning object" );
         return;
      }

      const name = this.tuningObject.get( "name" );
      outlet( 1, name );

      const noteTunings = JSON.parse( this.tuningObject.getstring( "note_tunings" ) );
      this.frequencies = noteTunings.note_tunings;
      let pseudoOctaveInCents = this.tuningObject.get( "pseudo_octave_in_cents" );
      this.frequencies = this.frequencies.map( ( freq ) => freq / pseudoOctaveInCents );

      let lowestNotePitch = new Pitch( this.tuningObject.getstring( "lowest_note" ) );
      let referencePitch = new Pitch( this.tuningObject.getstring( "reference_pitch" ) );
      //let highestNote = new Pitch( this.tuningObject.getstring( "highest_note" ) );

      this.refFrequency = referencePitch.frequency;

      let lowestNote = lowestNotePitch.indexInOctave + ( lowestNotePitch.octave * this.frequencies.length );
      this.refNote = referencePitch.indexInOctave + ( referencePitch.octave * this.frequencies.length ) - lowestNote;
   }

   process( note )
   {
      if ( ( this.tuningObject == null ) )
         this.read();

      let noteDiff = note - this.refNote;
      let octaveDiff = noteDiff % this.frequencies.length;
      while ( octaveDiff < 0 )
         octaveDiff += this.frequencies.length;

      let octave = Math.floor( noteDiff / this.frequencies.length );
      let octaveFrequency = this.refFrequency * Math.pow( 2, octave );

      let freqDiff = this.frequencies[ octaveDiff ];
      let value = octaveFrequency * Math.pow( 2, freqDiff );

      return value;
   }
}

//---------------------------------

let tuningSystem = new TuningSystem();

function bang()
{
   tuningSystem.read();
}

function msg_int( note )
{
   let value = tuningSystem.process( note );
   outlet( 0, value );
}



