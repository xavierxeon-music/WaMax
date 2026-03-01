#ifndef McLissajousH
#define McLissajousH

#include "c74_min.h"
using namespace c74::min;

class McBase : public object<McBase>
{
public:
   McBase(const atoms& args = {}) {}
};

class McLissajous : public object<McLissajous>, public mc_operator<>
{
public:
   MIN_DESCRIPTION{"generate Lissajous figures"};

public:
   McLissajous(const atoms& args = {});

public:
   void operator()(audio_bundle input, audio_bundle output);

private:
   atoms maxClassSetupFunction(const atoms& args, const int inlet);

   static long compileMultChannelOutputCount(c74::max::t_object* x, long index, long count);
   static long inputChanged(c74::max::t_object* x, long index, long count);

   atoms aFunction(const atoms& args, const int inlet);
   atoms aOffsetFunction(const atoms& args, const int inlet);
   atoms bFunction(const atoms& args, const int inlet);

private:
   attribute<int> chans; // must have this name and type
   message<> maxclassSetup;

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
