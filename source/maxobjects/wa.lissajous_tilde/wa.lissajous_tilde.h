#ifndef LissajousH
#define LissajousH

#include "c74_min.h"
using namespace c74::min;

class Lissajous : public object<Lissajous>, public sample_operator<1, 2>
{
public:
   MIN_DESCRIPTION{"generate Lissajous figures"};

public:
   Lissajous(const atoms& args = {});

public:
   samples<2> operator()(sample phase);

private:
   atoms aFunction(const atoms& args, const int inlet);
   atoms aOffsetFunction(const atoms& args, const int inlet);
   atoms bFunction(const atoms& args, const int inlet);

private:
   inlet<> phasorInput;
   outlet<> xOutput;
   outlet<> yOutput;

   message<> aMessage;
   message<> aOffsetMessage;
   message<> bMessage;

   double a;
   double aOffset;
   double b;
};

#endif // LissajousH
