#ifndef MidiToPulseH
#define MidiToPulseH

#include "c74_min.h"
using namespace c74::min;

class MidiToPulse : public object<MidiToPulse>
{
public:
   MIN_DESCRIPTION{"description"};

public:
   MidiToPulse(const atoms& args = {});
};

#endif // MidiToPulseH
