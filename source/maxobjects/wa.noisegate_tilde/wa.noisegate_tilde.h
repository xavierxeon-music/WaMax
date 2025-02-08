#ifndef NoiseGateH
#define NoiseGateH

#include "c74_min.h"
using namespace c74::min;

class NoiseGate : public object<NoiseGate>
{
public:
   MIN_DESCRIPTION{"description"};

public:
   NoiseGate(const atoms& args = {});
};

#endif // NoiseGateH
