#include "mc.wa.noisegate_tilde.h"

#include <MaxPatcher.h>

static const int msLength = 10;

McNoiseGate::McNoiseGate(const atoms& args)
   : MultichannelObject<McNoiseGate>()
   , threshold{this, "threshold", 0.005}
   , input(this, "input", "multichannelsignal")
   , output(this, "output", "multichannelsignal")
   , peakOutlet(this, "peak", "multichannelsignal")
   , activeOutlet(this, "active", "multichannelsignal")
   , dspSetup{this, "dspsetup", Max::Patcher::minBind(this, &McNoiseGate::dspSetupFunction)}
   , buffer()
{
   updateBuffer();
}

void McNoiseGate::operator()(audio_bundle input, audio_bundle output)
{
   if (input.channel_count() * 3 != output.channel_count())
   {
      cerr << "McNoiseGate: input and output channel count mismatch" << input.channel_count() << " " << output.channel_count() << endl;
      return;
   }

   const int channelCount = input.channel_count();
   for (int channelIndex = 0; channelIndex < channelCount; channelIndex++)
   {
      const int peakChannelIndex = channelIndex + (1 * channelCount);
      const int activeChannelIndex = channelIndex + (2 * channelCount);

      double* in = input.samples(channelIndex);
      double* out = output.samples(channelIndex);
      double* outPeak = output.samples(peakChannelIndex);
      double* outActive = output.samples(activeChannelIndex);

      for (int counter = 0; counter < input.frame_count(); counter++)
      {
         const sample value = in[counter];
         buffer[channelIndex].tapin(value, true);
      }

      const sample peak = buffer[channelIndex].peak();
      const bool active = (threshold >= peak);

      for (int counter = 0; counter < input.frame_count(); counter++)
      {
         out[counter] = active ? 0.0 : in[counter];
         outPeak[counter] = peak;
         outActive[counter] = active ? 1.0 : 0.0;
      }
   }
}

void McNoiseGate::setChannelCount(long index, int count)
{
   (void)index;
   chans = count;

   updateBuffer();
}

void McNoiseGate::updateBuffer()
{
   const int bufferSize = msLength * samplerate() / 1000;
   buffer.resize(chans, SampleDelay(bufferSize));
}

atoms McNoiseGate::dspSetupFunction(const atoms& args, const int inlet)
{
   //cout << "sample rate = " << samplerate() << endl;
   updateBuffer();

   return {};
}

MIN_EXTERNAL(McNoiseGate);
