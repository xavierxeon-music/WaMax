#ifndef PeaksH
#define PeaksH

#include "c74_min.h"
using namespace c74::min;

class Peaks : public object<Peaks>
{
public:
   MIN_DESCRIPTION{"description"};

public:
   Peaks(const atoms& args = {});
};

#endif // PeaksH
