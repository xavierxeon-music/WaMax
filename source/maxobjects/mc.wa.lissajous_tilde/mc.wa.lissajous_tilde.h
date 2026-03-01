#ifndef McLissajousH
#define McLissajousH

#include "c74_min.h"
using namespace c74::min;

#include <MultichannelObject.h>

class McLissajous : public MultichannelObject<McLissajous>
{
public:
   MIN_DESCRIPTION{"generate Lissajous figures"};

public:
   McLissajous(const atoms& args = {});

public:
   void operator()(audio_bundle input, audio_bundle output) override;

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

#endif // McLissajousH
