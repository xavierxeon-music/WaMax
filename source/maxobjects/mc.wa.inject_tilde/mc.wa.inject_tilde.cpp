#include "mc.wa.inject_tilde.h"

#include <MaxPatcher.h>

McInject::McInject(const atoms& args)
   : object<McInject>()
   , mc_operator<>()
   , chans{this, "chans", 1, range{1, 1024}}
   , target{this, "target", 1}
   , input(this, "input", "multichannelsignal")
   , single(this, "single", "signal")
   , output(this, "output", "multichannelsignal")
   , maxclassSetup{this, "maxclass_setup", Max::Patcher::minBind(this, &McInject::maxClassSetupFunction)}
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

atoms McInject::maxClassSetupFunction(const atoms& args, const int inlet)
{
   c74::max::t_class* c = args[0];
   c74::max::class_addmethod(c, (c74::max::method)McInject::compileMultChannelOutputCount, "multichanneloutputs", c74::max::A_CANT, 0);
   c74::max::class_addmethod(c, (c74::max::method)McInject::inputChanged, "inputchanged", c74::max::A_CANT, 0);

   return {};
}

long McInject::compileMultChannelOutputCount(c74::max::t_object* x, long index, long count)
{
   minwrap<McInject>* ob = (minwrap<McInject>*)(x);
   if (0 == index)
      return ob->m_min_object.chans;
   return 1;
}

long McInject::inputChanged(c74::max::t_object* x, long index, long count)
{
   minwrap<McInject>* ob = (minwrap<McInject>*)(x);
   if (0 == index)
      ob->m_min_object.chans = count;
   return true;
}

MIN_EXTERNAL(McInject);
