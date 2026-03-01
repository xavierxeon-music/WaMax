#include "mc.wa.extract_tilde.h"

#include <MaxPatcher.h>

McExtract::McExtract(const atoms& args)
   : MultichannelObject<McExtract>()
   , target{this, "target", 1}
   , strip{this, "strip", false}
   , input(this, "input", "multichannelsignal")
   , output(this, "output", "multichannelsignal")
   , single(this, "single", "signal")
{
}

void McExtract::operator()(audio_bundle input, audio_bundle output)
{
   if (input.channel_count() + 1 != output.channel_count())
   {
      cerr << "McExtract: input and output channel count mismatch " << input.channel_count() << " " << output.channel_count() << endl;
      return;
   }

   const int channelCount = input.channel_count();
   double* single = output.samples(channelCount);
   for (int channelIndex = 0; channelIndex < channelCount; channelIndex++)
   {
      double* in = input.samples(channelIndex);
      double* out = output.samples(channelIndex);

      for (int counter = 0; counter < input.frame_count(); counter++)
      {
         if (channelIndex + 1 == target)
         {
            single[counter] = in[counter];
            if (strip)
               out[counter] = 0.0;
            else
               out[counter] = in[counter];
         }
         else
         {
            out[counter] = in[counter];
         }
      }
   }
}

int McExtract::getChannelCount(long index) const
{
   if (0 == index)
      return chans;

   return 1;
}

void McExtract::setChannelCount(long index, int count)
{
   if (0 == index)
      chans = count;
}

MIN_EXTERNAL(McExtract);
