#ifndef PulseToMidiH
#define PulseToMidiH

#include "c74_min.h"
using namespace c74::min;

class PulseToMidi : public object<PulseToMidi>, public vector_operator<>
{
public:
   MIN_DESCRIPTION{"pulse to midi converter"};

public:
   PulseToMidi(const atoms& args = {});

public:
   void operator()(audio_bundle input, audio_bundle output);

private:
   inlet<> input;
   outlet<> output;
};

#endif // PulseToMidiH
