#include "wa.pulse2midi_tilde.h"

PulseToMidi::PulseToMidi(const atoms& args)
   : object<PulseToMidi>()
   , sample_operator<1, 0>()
   , input{this, "(signal) pulse", "signal"}
   , output{this, "(int) midi"}
{
}

void PulseToMidi::operator()(sample in)
{
}

MIN_EXTERNAL(PulseToMidi);
