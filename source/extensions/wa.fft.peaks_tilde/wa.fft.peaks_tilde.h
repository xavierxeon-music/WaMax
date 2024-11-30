#ifndef FourierPeaksH
#define FourierPeaksH

#include "c74_min.h"
using namespace c74::min;

namespace c74
{
   namespace max
   {
#include <r_pfft.h> // public pfft struct in r_pfft.h
   } // namespace max
} // namespace c74

#include "Peak.h"

class FourierPeaks : public object<FourierPeaks>, public sample_operator<2, 0>
{
public:
   MIN_DESCRIPTION{"description"};

public:
   FourierPeaks(const atoms& args = {});

public:
   sample operator()(sample amplitude, sample bin);

private:
   void setup();
   atoms calculateFunction(const atoms& args, const int inlet);

private:
   inlet<> inputAmplitude;
   inlet<> inputBin;
   outlet<> content;
   message<> bangMessage;

   Peak::List peaks;
   int peakCount;
};

#endif // FourierPeaksH
