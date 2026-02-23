#include "wa.lissajous_tilde.h"

#include <MaxPatcher.h>

Lissajous::Lissajous(const atoms& args)
   : object<Lissajous>()
   , phasorInput{this, "(signal) phasor signal", "signal"}
   , xOutput{this, "(signal) x signal", "signal"}
   , yOutput{this, "(signal) y signal", "signal"}
   , aMessage{this, "a", "a value", Max::Patcher::minBind(this, &Lissajous::aFunction)}
   , aOffsetMessage{this, "offset", "offset value", Max::Patcher::minBind(this, &Lissajous::aOffsetFunction)}
   , bMessage{this, "b", "b value", Max::Patcher::minBind(this, &Lissajous::bFunction)}
   , a(1.0)
   , aOffset(0.0)
   , b(1.0)
{
   if (args.size() > 0)
      a = args[0];
   if (args.size() > 1)
      aOffset = args[1];
   if (args.size() > 2)
      b = args[2];
}

// see https://en.wikipedia.org/wiki/Lissajous_curve
samples<2> Lissajous::operator()(sample phase)
{
   phase = (2.0 * M_PI * phase);

   const double x = sin(a * phase + aOffset);
   const double y = sin(b * phase);

   return {x, y};
}

atoms Lissajous::aFunction(const atoms& args, const int inlet)
{
   a = args[0];
   return {};
}

atoms Lissajous::aOffsetFunction(const atoms& args, const int inlet)
{
   aOffset = args[0];
   return {};
}

atoms Lissajous::bFunction(const atoms& args, const int inlet)
{
   b = args[0];
   return {};
}

MIN_EXTERNAL(Lissajous);
