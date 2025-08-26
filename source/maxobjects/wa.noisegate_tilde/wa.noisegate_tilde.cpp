#include "wa.noisegate_tilde.h"

#include <MaxPatcher.h>

static const int msLength = 10;

NoiseGate::NoiseGate(const atoms& args)
   : object<NoiseGate>()
   , vector_operator<>()
   , threshold{this, "threshold", 0.005}
   , inletList()
   , outletList()
   , buffer(0)
   , dspsetup{this, "dspsetup", Max::Patcher::minBind(this, &NoiseGate::dspSetupFunction)}
   , bufferSize(0)
   , portCount(0)
{
   bufferSize = msLength * samplerate() / 1000;
   portCount = args[0];
   if (0 == portCount)
      portCount = 1;

   for (auto i = 0; i < portCount; ++i)
   {
      const std::string counter = std::to_string(i + 1);
      Inlet an_inlet = std::make_unique<inlet<>>(this, "signal " + counter, "signal");
      inletList.push_back(std::move(an_inlet));

      Outlet an_outlet = std::make_unique<outlet<>>(this, "signal " + counter, "signal");
      outletList.push_back(std::move(an_outlet));
   }

   // peak and active as last ports, can not be seperate outlets
   Outlet peakOutlet = std::make_unique<outlet<>>(this, "peak", "signal");
   outletList.push_back(std::move(peakOutlet));

   Outlet activeOutlet = std::make_unique<outlet<>>(this, "active", "signal");
   outletList.push_back(std::move(activeOutlet));

   // average over all inputs
   buffer = SampleDelay(bufferSize * portCount);
}

void NoiseGate::operator()(audio_bundle input, audio_bundle output)
{
   const int peakChannel = input.channel_count();
   const int activeChannel = peakChannel + 1;
   double* outPeak = output.samples(peakChannel);
   double* outActive = output.samples(activeChannel);

   for (int channel = 0; channel < input.channel_count(); channel++)
   {
      double* in = input.samples(channel);
      double* out = output.samples(channel);
      for (int counter = 0; counter < input.frame_count(); counter++)
      {
         const sample value = in[counter];
         buffer.tapin(value, true);
      }

      const sample peak = buffer.peak();
      const bool active = (threshold >= peak);

      for (int counter = 0; counter < input.frame_count(); counter++)
      {
         out[counter] = active ? 0.0 : in[counter];
         outPeak[counter] = peak;
         outActive[counter] = active ? 1.0 : 0.0;
      }
   }
}

atoms NoiseGate::dspSetupFunction(const atoms& args, const int inlet)
{
   //cout << "sample rate = " << samplerate() << endl;
   bufferSize = msLength * samplerate() / 1000;

   buffer = SampleDelay(bufferSize * portCount);

   return {};
}

MIN_EXTERNAL(NoiseGate);
