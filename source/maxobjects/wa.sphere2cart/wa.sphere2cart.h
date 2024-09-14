
#ifndef WaSphere2CartH
#define WaSphere2CartH

#include "c74_min.h"
using namespace c74::min;

class Sphere2Cart : public object<Sphere2Cart>
{
public:
   MIN_DESCRIPTION{"convert sphercal corrdinates to 3d carteasion"};

public:
   Sphere2Cart();

public:
   inlet<> input;
   outlet<> output;
   message<> listMessage;

private:
   atoms listFunction(const atoms& args, const int inlet);
};

#endif // NOT  WaSphere2CartH
