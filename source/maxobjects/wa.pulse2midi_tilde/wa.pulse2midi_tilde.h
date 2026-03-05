#ifndef PulseToMidiH
#define PulseToMidiH

#include "c74_min.h"
using namespace c74::min;

class PulseToMidi : public object<PulseToMidi>
{
public:
   MIN_DESCRIPTION{"description"};

public:
   PulseToMidi(const atoms& args = {});
};

#endif // PulseToMidiH
