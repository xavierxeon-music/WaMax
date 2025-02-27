#include "wa.mixdown_tilde.h"

#include <MaxPatcher.h>

MixDown::MixDown(const atoms& args)
   : object<MixDown>()
   , vector_operator<>()
   , normalize{this, "normalize", true}
   , inletList()
   , outletList()
{
   int portCount = 1;
   if (args.size() > 0)
   {
      portCount = args[0];
      if (0 == portCount)
         portCount = 1;
   }

   for (auto i = 0; i < portCount; ++i)
   {
      const std::string counter = std::to_string(i + 1);
      Inlet an_inlet = std::make_unique<inlet<>>(this, "signal " + counter, "signal");
      inletList.push_back(std::move(an_inlet));
   }

   Outlet mixOutlet = std::make_unique<outlet<>>(this, "singal", "signal");
   outletList.push_back(std::move(mixOutlet));
}

void MixDown::operator()(audio_bundle input, audio_bundle output)
{
   const int channelCount = input.channel_count();

   for (int counter = 0; counter < input.frame_count(); counter++)
   {
      sample sum = 0.0;
      for (int channel = 0; channel < channelCount; channel++)
      {
         double* in = input.samples(channel);
         sum += in[counter];
      }

      if (normalize)
         sum /= channelCount;

      double* out = output.samples(0);
      out[counter] = sum;
   }
}

MIN_EXTERNAL(MixDown);
