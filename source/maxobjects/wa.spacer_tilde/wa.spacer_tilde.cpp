#include "wa.spacer_tilde.h"

#include <patcher.h>

Spacer::Spacer()
   : object<Spacer>()
   , sample_operator<1, 2>()
   , input{this, "(signal) signal"}
   , leftOutput{this, "(signal) left signal", "signal"}
   , rightOutput{this, "(signal) right signal", "signal"}
   , last(0)
{
}

samples<2> Spacer::operator()(sample x)
{
   sample left = x;
   sample right = x;

   return {left, right};
}

MIN_EXTERNAL(Spacer);
