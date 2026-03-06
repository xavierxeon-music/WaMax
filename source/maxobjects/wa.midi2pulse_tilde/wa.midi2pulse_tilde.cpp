#include "wa.midi2pulse_tilde.h"

#include <ByteFours.h>
#include <MaxPatcher.h>

MidiToPulse::MidiToPulse(const atoms& args)
   : object<MidiToPulse>()
   , sample_operator<0, 1>()
   , input{this, "(int) midi value"}
   , output{this, "(signal) pulse", "signal"}
   , intMessage{this, "int", "integer value.", Max::Patcher::minBind(this, &MidiToPulse::intFunction)}
   , buffer(0)
{
}

samples<1> MidiToPulse::operator()()
{
   sample out;

   ByteFours converter(buffer);

   return {out};
}

atoms MidiToPulse::intFunction(const atoms& args, const int inlet)
{
   buffer = args[0];
   if (buffer < 0)
      buffer = 0;
   if (buffer > 255)
      buffer = 255;

   return {};
}

MIN_EXTERNAL(MidiToPulse);
