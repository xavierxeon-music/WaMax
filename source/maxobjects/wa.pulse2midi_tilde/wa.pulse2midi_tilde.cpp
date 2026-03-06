#include "wa.pulse2midi_tilde.h"

#include <ByteFours.h>

PulseToMidi::PulseToMidi(const atoms& args)
   : object<PulseToMidi>()
   , sample_operator<1, 0>()
   , input{this, "(signal) pulse", "signal"}
   , output{this, "(int) midi"}
{
}

void PulseToMidi::operator()(sample in)
{
   ByteFours converter;
}

MIN_EXTERNAL(PulseToMidi);
