
#ifndef WaSphere2CartH
#define WaSphere2CartH

#include "c74_min.h"
using namespace c74::min;

#include <linalg.h>

class Sphere2Cart : public object<Sphere2Cart>
{
public:
   MIN_DESCRIPTION{"convert sphercal corrdinates to 3d carteasion"};

public:
   Sphere2Cart(const atoms& args = {});

private:
   atoms azFunction(const atoms& args, const int inlet);
   atoms elFunction(const atoms& args, const int inlet);
   atoms radiusFunction(const atoms& args, const int inlet);
   atoms listFunction(const atoms& args, const int inlet);
   atoms calculateFunction(const atoms& args, const int inlet);
   void calcluate();

private:
   Linalg::Vector3 spherical;
   outlet<> output;
   attribute<bool> asDegrees;
   message<> azMessage;
   message<> elMessage;
   message<> radiusMessage;
   message<> listMessage;
   message<> bangMessage;
   message<> doubleClick;
};

#endif // NOT  WaSphere2CartH
