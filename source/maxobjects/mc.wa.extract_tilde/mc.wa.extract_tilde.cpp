#include "mc.wa.extract_tilde.h"

#include <MaxPatcher.h>

McExtract::McExtract(const atoms& args)
   : object<McExtract>()
   , mc_operator<>()
   , chans{this, "chans", 1, range{1, 1024}}
   , maxclassSetup{this, "maxclass_setup", Max::Patcher::minBind(this, &McExtract::maxClassSetupFunction)}
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

atoms McExtract::maxClassSetupFunction(const atoms& args, const int inlet)
{
   c74::max::t_class* c = args[0];
   c74::max::class_addmethod(c, (c74::max::method)McExtract::compileMultChannelOutputCount, "multichanneloutputs", c74::max::A_CANT, 0);
   c74::max::class_addmethod(c, (c74::max::method)McExtract::inputChanged, "inputchanged", c74::max::A_CANT, 0);

   return {};
}

long McExtract::compileMultChannelOutputCount(c74::max::t_object* x, long index, long count)
{
   minwrap<McExtract>* ob = (minwrap<McExtract>*)(x);
   if (0 == index)
      return ob->m_min_object.chans;

   return 1;
}

long McExtract::inputChanged(c74::max::t_object* x, long index, long count)
{
   minwrap<McExtract>* ob = (minwrap<McExtract>*)(x);
   if (0 == index)
      ob->m_min_object.chans = count;

   return true;
}
MIN_EXTERNAL(McExtract);
