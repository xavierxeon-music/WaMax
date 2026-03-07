#include "wa.pulse2midi_tilde.h"

PulseToMidi::PulseToMidi(const atoms& args)
   : object<PulseToMidi>()
   , vector_operator<>()
   , input{this, "(signal) pulse", "signal"}
   , output{this, "(int) midi"}
   , gapCounter(0)
   , index(0)
   , buffer{ByteFours::FoursValue::O, ByteFours::FoursValue::O, ByteFours::FoursValue::O, ByteFours::FoursValue::O}
{
}

void PulseToMidi::operator()(audio_bundle inputs, audio_bundle outputs)
{
   double* in = inputs.samples(0); // first channel only
   for (int frame = 0; frame < inputs.frame_count(); ++frame)
   {
      const sample value = in[frame];
      if (!ByteFours::isValidSample(value))
      {
         continue;
      }

      buffer[index] = ByteFours::fromSample(value);
      index++;

      if (index >= 4)
      {
         index = 0;
         ByteFours fours(buffer[0], buffer[1], buffer[2], buffer[3]);
         uint8_t byte = fours;
         output.send(byte);
      }
   }
}

MIN_EXTERNAL(PulseToMidi);
