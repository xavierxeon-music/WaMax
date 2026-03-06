#ifndef PulseToMidiH
#define PulseToMidiH

#include "c74_min.h"
using namespace c74::min;

class PulseToMidi : public object<PulseToMidi>, public sample_operator<1, 0>
{
public:
   MIN_DESCRIPTION{"pulse to midi converter"};

public:
   PulseToMidi(const atoms& args = {});

public:
   void operator()(sample in);

private:
   inlet<> input;
   outlet<> output;
};

#endif // PulseToMidiH
