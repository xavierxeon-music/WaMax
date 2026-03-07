#include "wa.midi2pulse_tilde.h"

#include <MaxPatcher.h>

MidiToPulse::MidiToPulse(const atoms& args)
   : object<MidiToPulse>()
   , vector_operator<>()
   , input{this, "(int) midi value"}
   , output{this, "(signal) pulse", "signal"}
   , intMessage{this, "int", "integer value.", Max::Patcher::minBind(this, &MidiToPulse::intFunction)}
   , buffer(0)
{
}

void MidiToPulse::operator()(audio_bundle input, audio_bundle output)
{
   //cout << "input " << input.channel_count() << " channels, " << input.frame_count() << " frames" << endl;
   //cout << ", output " << output.channel_count() << " channels, " << output.frame_count() << " frames" << endl;

   double* out = output.samples(0);
   for (int frame = 0; frame < output.frame_count(); ++frame)
   {
      out[frame] = 0.0;
   }
}

atoms MidiToPulse::intFunction(const atoms& args, const int inlet)
{
   buffer = args[0];
   cout << "received int: " << buffer << endl;

   return {};
}

MIN_EXTERNAL(MidiToPulse);
