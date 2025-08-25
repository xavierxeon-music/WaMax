#include "wa.pass.spring_tilde.h"

#include <AudioBlock.h>

PassSpring::PassSpring(const atoms& args)
   : object<PassSpring>()
   , vector_operator<>()
   , outletList()
   , audioBlocks()
   , timestamp(0)
{
   QString name = "dummy";
   if (args.size() > 0)
   {
      const std::string deviceName = std::string(args[0]);
      name = QString::fromStdString(deviceName);
   }

   int portCount = 1;
   if (args.size() > 1)
   {
      portCount = args[1];
      if (0 == portCount)
         portCount = 1;
   }

   for (int i = 0; i < portCount; ++i)
   {
      const std::string counter = std::to_string(i + 1);
      Outlet an_outlet = std::make_unique<outlet<>>(this, "signal " + counter, "signal");
      outletList.push_back(std::move(an_outlet));

      AudioBlock* audioBlock = AudioBlock::create(name, i + 1);
      audioBlocks.push_back(audioBlock);
   }
}

void PassSpring::operator()(audio_bundle input, audio_bundle output)
{
   const int channelCount = output.channel_count();
   const int frameSize = output.frame_count();

   for (int channel = 0; channel < channelCount; channel++)
   {
      double* out = output.samples(channel);
      AudioBlock* audioBlock = audioBlocks[channel];
      if (!audioBlock || audioBlock->timestamp <= timestamp)
      {
         std::memset(out, 0, frameSize * sizeof(double));
         continue;
      }

      std::memcpy(out, audioBlock->data, frameSize * sizeof(double));
   }
}

MIN_EXTERNAL(PassSpring);
