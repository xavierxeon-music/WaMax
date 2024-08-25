
#ifndef WaDerivativeH
#define WaDerivativeH

#include "c74_min.h"
using namespace c74::min;

class Derivative : public object<Derivative>, public sample_operator<1, 1>
{
public:
   MIN_DESCRIPTION{"7bit list to int"};

public:
   Derivative();

public:
   inlet<> input;
   outlet<> output;
   message<> dspsetup;

   sample operator()(sample x);

private:
   atoms setupFunction(const atoms& args, const int inlet);

private:
   sample last;
   number inverseStep;
};

#endif // NOT  WaDerivativeH
