#include "wa.fft.peaks_tilde.h"

namespace c74
{
   namespace max
   {
#include <r_pfft.h> // public pfft struct in r_pfft.h
   } // namespace max
} // namespace c74

#include <MaxPatcher.h>

FourierPeaks::FourierPeaks(const atoms& args)
   : object<FourierPeaks>()
   , sample_operator<2, 0>()
   , inputAmplitude{this, "(signal) amplitude, bang"}
   , inputBin{this, "(signal) bin"}
   , content{this, "(list) peaks"}
   , dspsetup(this, "dspsetup", Max::Patcher::minBind(this, &FourierPeaks::dspSetupFunction))
   , bangMessage(this, "bang", "", Max::Patcher::minBind(this, &FourierPeaks::calculateFunction))
   , peaks()
   , peakCount(1)
{
   if (args.size() > 0)
      peakCount = args[0];
}

sample FourierPeaks::operator()(sample amplitude, sample bin)
{
   const int index = std::round(bin);
   //          peaks[index] = Peak{index, value};

   if (minIndex == -1 || index < minIndex)
      minIndex = index;
   if (maxIndex == -1 || index > maxIndex)
      maxIndex = index;

   return 0;
}

atoms FourierPeaks::dspSetupFunction(const atoms& args, const int inlet)
{
   // fft stuff
   c74::max::t_symbol* symbol_pfft = c74::max::gensym("__pfft~__");
   if (!symbol_pfft)
   {
      cout << "wa.fft.peaks~ only functions inside a pfft~" << endl;
      return {};
   }

   c74::max::t_pfftpub* pfft = (c74::max::t_pfftpub*)symbol_pfft->s_thing;

   int fullspect = pfft->x_fullspect ? 1 : 0;
   long ffthop = pfft->x_ffthop;
   long fftsize = pfft->x_fftsize;

   cout << "fullspect: " << fullspect << endl;
   cout << "ffthop: " << ffthop << endl;
   cout << "fftsize: " << fftsize << endl;

   peaks.resize(fftsize);

   return {};
}

atoms FourierPeaks::calculateFunction(const atoms& args, const int inlet)
{
   //std::sort(peaks.begin(), peaks.end(), std::greater<Peak>());
   cout << "minIndex: " << minIndex << endl;
   cout << "maxIndex: " << maxIndex << endl;

   if (peakCount > peaks.size())
      return {};

   atoms result(peakCount, 0);
   for (int i = 0; i < peakCount; i++)
   {
      //result[i] = peaks[i].index;
   }
   content.send(result);
   return {};
}

MIN_EXTERNAL(FourierPeaks);
