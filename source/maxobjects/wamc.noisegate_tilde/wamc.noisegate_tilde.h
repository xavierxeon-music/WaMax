#ifndef McNoiseGateH
#define McNoiseGateH

#include "c74_min.h"
using namespace c74::min;

#include <SampleDelay.h>

// for mc wrapper see https://cycling74.com/forums/mc-support-with-min-api-for
// see https://github.com/acids-ircam/nn_tilde

class McNoiseGate : public object<NoiseGate>, public vector_operator<>
{
public:
   MIN_DESCRIPTION{"description"};

public:
   McNoiseGate(const atoms& args = {});

public:
   void operator()(audio_bundle input, audio_bundle output);

private:
   atoms dspSetupFunction(const atoms& args, const int inlet);
   atoms maxClassSetupFunction(const atoms& args, const int inlet);

   static long compileMultChannelOutputCount(c74::max::t_object* x, long index, long count);
   static long inputChanged(c74::max::t_object* x, long index, long count);

private:
   attribute<int> chans; // must have this name and type
   attribute<double> threshold;

   inlet<> input;
   outlet<> output;
   outlet<> peakOutlet;
   outlet<> activeOutlet;

   SampleDelay buffer;

   message<> dspSetup;
   message<> maxclassSetup;

   int bufferSize;
};

#endif // McNoiseGateH
