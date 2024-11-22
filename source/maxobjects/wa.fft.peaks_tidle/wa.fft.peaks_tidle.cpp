#include "wa.fft.peaks_tidle.h"

FourierPeaks::FourierPeaks(const atoms& args)
   : object<FourierPeaks>()
   , peaks()
{
   std::srand(std::time(nullptr));

   peaks.resize(100);

   for (int index = 0; index < 100; index++)
   {
      const double value = std::rand() / static_cast<double>(RAND_MAX);
      peaks[index] = Peak{index, value};
   }

   std::sort(peaks.begin(), peaks.end(), std::greater<Peak>());
}

MIN_EXTERNAL(FourierPeaks);
