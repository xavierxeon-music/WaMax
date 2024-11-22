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
   Peak::List peaks;
};

#endif // FourierPeaksH
