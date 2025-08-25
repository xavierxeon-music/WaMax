#include "wa.pipe.tap_tilde.h"

#include <AudioBlock.h>

PipeTap::PipeTap(const atoms& args)
   : object<PipeTap>()
   , vector_operator<>()
   , outletList()
   , audioBlocks()
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

      AudioBlock* audioBlock = new AudioBlock(name, i + 1, true);
      audioBlocks.push_back(audioBlock);

      if (!audioBlock->getErrorString().isEmpty())
         cerr << "wa.pass.spring " << name.toStdString() << " @ " << i << " " << audioBlock->getErrorString().toStdString() << endl;
   }
}

void PipeTap::operator()(audio_bundle input, audio_bundle output)
{
   const int channelCount = output.channel_count();
   const int frameSize = output.frame_count();

   for (int channel = 0; channel < channelCount; channel++)
   {
      double* out = output.samples(channel);
      AudioBlock* audioBlock = audioBlocks[channel];
      audioBlock->copyTo(out, frameSize);
   }
}

MIN_EXTERNAL(PipeTap);
