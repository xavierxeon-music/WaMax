#include "wa.fft.peaks_tilde.h"

#include <MaxPatcher.h>

FourierPeaks::FourierPeaks(const atoms& args)
   : object<FourierPeaks>()
   , sample_operator<2, 0>()
   , inputAmplitude{this, "(signal) amplitude, bang"}
   , inputBin{this, "(signal) bin"}
   , content{this, "(list) peaks"}
   , bangMessage(this, "bang", "get peaks", Max::Patcher::minBind(this, &FourierPeaks::calculateFunction))
   , peaks()
   , peakCount(1)
{
   if (args.size() > 0)
      peakCount = args[0];

   setup();
}

sample FourierPeaks::operator()(sample amplitude, sample bin)
{
   const int index = std::round(bin);
   peaks[index] = Peak{index, amplitude};

   return 0;
}

void FourierPeaks::setup()
{
   // fft stuff
   c74::max::t_symbol* symbol_pfft = c74::max::gensym("__pfft~__");
   if (!symbol_pfft)
   {
      cout << "wa.fft.peaks~ only functions inside a pfft~" << endl;
      return;
   }

   c74::max::t_pfftpub* pfft = (c74::max::t_pfftpub*)symbol_pfft->s_thing;
   if (!pfft)
   {
      cout << "wa.fft.peaks~ unable to find fft info" << endl;
      return;
   }
   const int ffthop = pfft->x_ffthop;
   peaks.resize(ffthop);

   //cout << peakCount << " peaks  of max " << ffthop << endl;
}

atoms FourierPeaks::calculateFunction(const atoms& args, const int inlet)
{
   if (peakCount > peaks.size())
      return {};

   atoms result(peakCount, 0);
   for (int i = 0; i < peakCount; i++)
   {
      result[i] = peaks[i].index;
   }
   content.send(result);
   return {};
}

MIN_EXTERNAL(FourierPeaks);
