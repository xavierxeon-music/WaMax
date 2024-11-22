#ifndef FourierPeaksH
#define FourierPeaksH

#include "c74_min.h"
using namespace c74::min;

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
   atoms dspSetupFunction(const atoms& args, const int inlet);
   atoms calculateFunction(const atoms& args, const int inlet);

private:
   inlet<> inputAmplitude;
   inlet<> inputBin;
   outlet<> content;
   message<> dspsetup;
   message<> bangMessage;

   Peak::List peaks;
   int peakCount;

   int minIndex = -1;
   int maxIndex = -1;
};

#endif // FourierPeaksH
