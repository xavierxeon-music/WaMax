#include "wa.derivative_tilde.h"

#include <common.h>

Derivative::Derivative()
   : object<Derivative>()
   , sample_operator<1, 1>()
   , input{this, "(signal) signal"}
   , output{this, "(signal) derivative of signal", "signal"}
   , dspsetup{this, "dspsetup", Patcher::minBind(this, &Derivative::setupFunction)}
   , last(0)
   , inverseStep(1)
{
}

sample Derivative::operator()(sample x)
{
   sample diff = (x - last) * inverseStep;
   last = x;
   return diff;
}

atoms Derivative::setupFunction(const atoms& args, const int inlet)
{
   inverseStep = args[0]; // sample rate
   //int vectorsize = args[1];

   return {};
}

MIN_EXTERNAL(Derivative);
