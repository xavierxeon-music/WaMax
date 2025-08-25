#include "wa.pass.drain_tilde.h"

#include <QDateTime>

PassDrain::PassDrain(const atoms& args)
   : object<PassDrain>()
   , vector_operator<>()
   , inletList()
   , audioBlocks()
{
   if (0 == args.size())
      return;

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
      Inlet an_inlet = std::make_unique<inlet<>>(this, "signal " + counter, "signal");
      inletList.push_back(std::move(an_inlet));

      AudioBlock* audioBlock = new AudioBlock(name, i + 1);
      audioBlocks.push_back(audioBlock);

      cout << "test " << i << " " << audioBlock->getErrorString().toStdString() << endl;
   }
}

void PassDrain::operator()(audio_bundle input, audio_bundle output)
{
   const int channelCount = input.channel_count();
   const int frameSize = input.frame_count();

   for (int channel = 0; channel < channelCount; channel++)
   {
      double* in = input.samples(channel);
      AudioBlock* audioBlock = audioBlocks[channel];
      audioBlock->copyFrom(in, frameSize);
   }
}

MIN_EXTERNAL(PassDrain);
