#ifndef PulseToMidiH
#define PulseToMidiH

#include "c74_min.h"
using namespace c74::min;

#include <ByteFours.h>

class PulseToMidi : public object<PulseToMidi>, public vector_operator<>
{
public:
   MIN_DESCRIPTION{"pulse to midi converter"};

public:
   PulseToMidi(const atoms& args = {});

public:
   void operator()(audio_bundle inputs, audio_bundle outputs);

private:
   inlet<> input;
   outlet<> output;

   uint8_t gapCounter;
   uint8_t index;
   ByteFours::FoursValue buffer[4];
};

#endif // PulseToMidiH
