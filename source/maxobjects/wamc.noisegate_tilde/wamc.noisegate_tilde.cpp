#include "wamc.noisegate_tilde.h"

McNoiseGate::McNoiseGate(const atoms& args)
   : object<McNoiseGate>()
   , vector_operator<>()
   , threshold{this, "threshold", 0.005}
   , chans{this, "chans", 1, range{1, 1024}}
   , input(this, "input", "multichannelsignal")
   , output(this, "output", "multichannelsignal")
   , peakOutlet(this, "peak", "multichannelsignal")
   , activeOutlet(this, "active", "multichannelsignal")
   , buffer(0)
   , dspSetup{this, "dspsetup", Max::Patcher::minBind(this, &NoiseGate::dspSetupFunction)}
   , maxclassSetup{this, "maxclass_setup", Max::Patcher::minBind(this, &NoiseGate::maxClassSetupFunction)}
   , bufferSize(0)
{
   bufferSize = msLength * samplerate() / 1000;

   buffer = SampleDelay(bufferSize * chans);
}

void McNoiseGate::operator()(audio_bundle input, audio_bundle output)
{
   /*
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
      */
}

atoms McNoiseGate::dspSetupFunction(const atoms& args, const int inlet)
{
   //cout << "sample rate = " << samplerate() << endl;
   bufferSize = msLength * samplerate() / 1000;

   buffer = SampleDelay(bufferSize * portCount);

   return {};
}

atoms McNoiseGate::maxClassSetupFunction(const atoms& args, const int inlet)
{
   c74::max::t_class* c = args[0];
   c74::max::class_addmethod(c, (c74::max::method)McNoiseGate::compileMultChannelOutputCount, "multichanneloutputs", c74::max::A_CANT, 0);

   c74::max::class_addmethod(c, (c74::max::method)McNoiseGate::inputChanged, "inputchanged", c74::max::A_CANT, 0);

   return {};
}

long McNoiseGate::compileMultChannelOutputCount(c74::max::t_object* x, long index, long count)
{
   minwrap<McNoiseGate>* ob = (minwrap<McNoiseGate>*)(x);
   return ob->m_min_object.chans;
}

long McNoiseGate::inputChanged(c74::max::t_object* x, long index, long count)
{
   minwrap<McNoiseGate>* ob = (minwrap<McNoiseGate>*)(x);
   return true;
}

MIN_EXTERNAL(McNoiseGate);
