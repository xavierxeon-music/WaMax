#ifndef McNoiseGateH
#define McNoiseGateH

#include "c74_min.h"
using namespace c74::min;

#include <MultichannelObject.h>
#include <SampleDelay.h>

class McNoiseGate : public MultichannelObject<McNoiseGate>
{
public:
   MIN_DESCRIPTION{"mc version of noisegate~"};

public:
   McNoiseGate(const atoms& args = {});

public:
   void operator()(audio_bundle input, audio_bundle output) override;
   void setChannelCount(long index, int count) override;

private:
   atoms dspSetupFunction(const atoms& args, const int inlet);
   void updateBuffer();

private:
   attribute<double> threshold;

   inlet<> input;
   outlet<> output;
   outlet<> peakOutlet;
   outlet<> activeOutlet;

   message<> dspSetup;
   std::vector<SampleDelay> buffer;
};

#endif // McNoiseGateH
