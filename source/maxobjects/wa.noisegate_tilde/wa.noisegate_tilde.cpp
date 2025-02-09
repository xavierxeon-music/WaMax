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

   for (auto i = 0; i < portCount; ++i)
   {
      const std::string counter = std::to_string(i + 1);
      Inlet an_inlet = std::make_unique<inlet<>>(this, "signal " + counter, "signal");
      inletList.push_back(std::move(an_inlet));

      Outlet an_outlet = std::make_unique<outlet<>>(this, "signal " + counter, "signal");
      outletList.push_back(std::move(an_outlet));
   }

   Outlet peakOutlet = std::make_unique<outlet<>>(this, "peak", "signal");
   outletList.push_back(std::move(peakOutlet));

   Outlet activeOutlet = std::make_unique<outlet<>>(this, "active", "signal");
   outletList.push_back(std::move(activeOutlet));

   buffer = SampleDelay(bufferSize * portCount);
}

void NoiseGate::operator()(audio_bundle input, audio_bundle output)
{
   const int peakChanngel = input.channel_count();
   const int activeChannel = peakChanngel + 1;

   for (int counter = 0; counter < input.frame_count(); counter++)
   {
      for (int channel = 0; channel < input.channel_count(); channel++)
      {
         double* in = input.samples(channel);
         const sample value = in[counter];
         buffer.tapin(value, true);
      }

      const sample peak = buffer.peak();
      double* outpeak = output.samples(peakChanngel);
      outpeak[counter] = peak;

      const bool active = (threshold >= peak);
      double* outActive = output.samples(activeChannel);
      outActive[counter] = active ? 1.0 : 0.0;

      for (int channel = 0; channel < input.channel_count(); channel++)
      {
         double* in = input.samples(channel);
         double* out = output.samples(channel);

         const sample value = in[counter];
         out[counter] = active ? 0.0 : value;
      }
   }
}

atoms NoiseGate::dspSetupFunction(const atoms& args, const int inlet)
{
   cout << "sample rate = " << samplerate() << endl;
   bufferSize = msLength * samplerate() / 1000;

   buffer = SampleDelay(bufferSize * portCount);

   return {};
}

MIN_EXTERNAL(NoiseGate);
