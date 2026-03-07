#include "wa.pulse2midi_tilde.h"

PulseToMidi::PulseToMidi(const atoms& args)
   : object<PulseToMidi>()
   , vector_operator<>()
   , input{this, "(signal) pulse", "signal"}
   , output{this, "(int) midi"}
{
}

void PulseToMidi::operator()(audio_bundle input, audio_bundle output)
{
   double* in = input.samples(0);
   for (int frame = 0; frame < input.frame_count(); ++frame)
   {
      const sample value = in[frame];
   }
}

MIN_EXTERNAL(PulseToMidi);
