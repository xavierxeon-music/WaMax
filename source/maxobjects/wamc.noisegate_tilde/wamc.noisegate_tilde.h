#ifndef McNoiseGateH
#define McNoiseGateH

#include "c74_min.h"
using namespace c74::min;

#include <SampleDelay.h>

class McNoiseGate : public object<McNoiseGate>, public mc_operator<>
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
   void updateBuffer();

   static long compileMultChannelOutputCount(c74::max::t_object* x, long index, long count);
   static long inputChanged(c74::max::t_object* x, long index, long count);

private:
   attribute<int> chans; // must have this name and type
   attribute<double> threshold;

   inlet<> input;
   outlet<> output;
   outlet<> peakOutlet;
   outlet<> activeOutlet;

   message<> dspSetup;
   message<> maxclassSetup;

   std::vector<SampleDelay> buffer;
};

#endif // McNoiseGateH
