#ifndef LissajousH
#define LissajousH

#include "c74_min.h"
using namespace c74::min;

class Lissajous : public object<Lissajous>, public sample_operator<1, 2>
{
public:
   MIN_DESCRIPTION{"description"};

public:
   Lissajous(const atoms& args = {});

public:
   samples<2> operator()(sample x);

private:
   atoms aFunction(const atoms& args, const int inlet);
   atoms phaseFunction(const atoms& args, const int inlet);
   atoms bFunction(const atoms& args, const int inlet);

private:
   inlet<> input;
   outlet<> xOutput;
   outlet<> yOutput;

   attribute<double> aValue;
   attribute<double> phaseValue;
   attribute<double> bValue;

   message<> aMessage;
   message<> phaseMessage;
   message<> bMessage;
};

#endif // LissajousH
