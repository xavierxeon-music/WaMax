#include "mc.wa.inject_tilde.h"

#include <MaxPatcher.h>

McInject::McInject(const atoms& args)
   : MultichannelObject<McInject>()
   , target{this, "target", 1}
   , input(this, "input", "multichannelsignal")
   , single(this, "single", "signal")
   , output(this, "output", "multichannelsignal")
{
}

void McInject::operator()(audio_bundle input, audio_bundle output)
{
   if (input.channel_count() != output.channel_count() + 1)
   {
      //cerr << "McInject: input and output channel count mismatch " << input.channel_count() - 1 << " " << output.channel_count() << endl;
      return;
   }

   const int channelCount = output.channel_count();
   double* single = input.samples(channelCount);
   for (int channelIndex = 0; channelIndex < channelCount; channelIndex++)
   {
      double* in = input.samples(channelIndex);
      double* out = output.samples(channelIndex);

      for (int counter = 0; counter < input.frame_count(); counter++)
      {
         if (channelIndex + 1 == target)
            out[counter] = single[counter];
         else
            out[counter] = in[counter];
      }
   }
}

int McInject::getChannelCount(long index) const
{
   if (0 == index)
      return chans;

   return 0;
}

void McInject::setChannelCount(long index, int count)
{
   if (0 == index)
      chans = count;
}

MIN_EXTERNAL(McInject);
