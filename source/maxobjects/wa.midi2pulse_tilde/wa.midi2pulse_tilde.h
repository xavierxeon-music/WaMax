#ifndef MidiToPulseH
#define MidiToPulseH

#include "c74_min.h"
using namespace c74::min;

class MidiToPulse : public object<MidiToPulse>, public sample_operator<0, 1>
{
public:
   MIN_DESCRIPTION{"midi to pulse converter"};

public:
   MidiToPulse(const atoms& args = {});

public:
   samples<1> operator()();

private:
   atoms intFunction(const atoms& args, const int inlet);

private:
   inlet<> input;
   outlet<> output;
   message<> intMessage;
   int buffer;
};

#endif // MidiToPulseH
