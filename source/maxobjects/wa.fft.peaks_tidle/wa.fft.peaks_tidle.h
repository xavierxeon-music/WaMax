#ifndef FourierPeaksH
#define FourierPeaksH

#include "c74_min.h"
using namespace c74::min;

#include "Peak.h"

class FourierPeaks : public object<FourierPeaks>
{
public:
   MIN_DESCRIPTION{"description"};

public:
   FourierPeaks(const atoms& args = {});

private:
   atoms dspSetupFunction(const atoms& args, const int inlet);

private:
   message<> dspsetup;
   Peak::List peaks;
};

#endif // FourierPeaksH
