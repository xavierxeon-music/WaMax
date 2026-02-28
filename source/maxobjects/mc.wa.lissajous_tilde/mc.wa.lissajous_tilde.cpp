#include "mc.wa.lissajous_tilde.h"

#include <MaxPatcher.h>

McLissajous::McLissajous(const atoms& args)
   : object<McLissajous>()
   , mc_operator<>()
   , chans{this, "chans", 1, range{1, 1024}}
   , maxclassSetup{this, "maxclass_setup", Max::Patcher::minBind(this, &McLissajous::maxClassSetupFunction)}
   , phasorInput{this, "(signal) phasor signal", "multichannelsignal"}
   , xOutput{this, "(signal) x signal", "multichannelsignal"}
   , yOutput{this, "(signal) y signal", "multichannelsignal"}
   , aMessage{this, "a", "a value", Max::Patcher::minBind(this, &McLissajous::aFunction)}
   , aOffsetMessage{this, "offset", "offset value", Max::Patcher::minBind(this, &McLissajous::aOffsetFunction)}
   , bMessage{this, "b", "b value", Max::Patcher::minBind(this, &McLissajous::bFunction)}
   , a(1.0)
   , aOffset(0.0)
   , b(1.0)
{
   if (args.size() > 0)
      a = args[0];
   if (args.size() > 1)
      aOffset = args[1];
   if (args.size() > 2)
      b = args[2];
}

// see https://en.wikipedia.org/wiki/Lissajous_curve
void McLissajous::operator()(audio_bundle input, audio_bundle output)
{
   if (2 * input.channel_count() != output.channel_count())
   {
      cerr << "Mc.Lissajous: input and output channel count mismatch " << input.channel_count() << " " << output.channel_count() << endl;
      return;
   }

   const int channelCount = input.channel_count();
   for (int channelIndex = 0; channelIndex < channelCount; channelIndex++)
   {
      double* phaseIn = input.samples(channelIndex);
      double* xOut = output.samples(channelIndex + 0);
      double* yOut = output.samples(channelIndex + channelCount);

      for (int counter = 0; counter < input.frame_count(); counter++)
      {
         const double phase = (2.0 * M_PI * phaseIn[counter]);

         xOut[counter] = sin(a * phase + aOffset);
         yOut[counter] = sin(b * phase);
      }
   }
}

atoms McLissajous::maxClassSetupFunction(const atoms& args, const int inlet)
{
   c74::max::t_class* c = args[0];
   c74::max::class_addmethod(c, (c74::max::method)McLissajous::compileMultChannelOutputCount, "multichanneloutputs", c74::max::A_CANT, 0);
   c74::max::class_addmethod(c, (c74::max::method)McLissajous::inputChanged, "inputchanged", c74::max::A_CANT, 0);

   return {};
}

long McLissajous::compileMultChannelOutputCount(c74::max::t_object* x, long index, long count)
{
   minwrap<McLissajous>* ob = (minwrap<McLissajous>*)(x);
   if (0 == index || 1 == index)
   {
      const int count = ob->m_min_object.chans;
      return count;
   }

   return 0;
}

long McLissajous::inputChanged(c74::max::t_object* x, long index, long count)
{
   minwrap<McLissajous>* ob = (minwrap<McLissajous>*)(x);
   if (0 == index)
      ob->m_min_object.chans = count;

   return true;
}

atoms McLissajous::aFunction(const atoms& args, const int inlet)
{
   a = args[0];
   return {};
}

atoms McLissajous::aOffsetFunction(const atoms& args, const int inlet)
{
   aOffset = args[0];
   return {};
}

atoms McLissajous::bFunction(const atoms& args, const int inlet)
{
   b = args[0];
   return {};
}

MIN_EXTERNAL(McLissajous);
