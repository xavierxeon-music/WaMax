#ifndef MidiToPulseH
#define MidiToPulseH

#include "c74_min.h"
using namespace c74::min;

class MidiToPulse : public object<MidiToPulse>, public vector_operator<>
{
public:
   MIN_DESCRIPTION{"midi to pulse converter"};

public:
   MidiToPulse(const atoms& args = {});

public:
   void operator()(audio_bundle input, audio_bundle output);

private:
   atoms intFunction(const atoms& args, const int inlet);

private:
   inlet<> input;
   outlet<> output;
   message<> intMessage;

   std::vector<uint8_t> buffer;
   uint16_t bufferSize;
};

#endif // MidiToPulseH
