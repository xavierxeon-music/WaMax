#include "wa.lissajous_tilde.h"

#include <MaxPatcher.h>

Lissajous::Lissajous(const atoms& args)
   : object<Lissajous>()
   , input{this, "(signal) frequency"}
   , xOutput{this, "(signal) x signal", "signal"}
   , yOutput{this, "(signal) y signal", "signal"}
   , aValue{this, "a", true}
   , phaseValue{this, "phase", true}
   , bValue{this, "b", true}
   , aMessage{this, "a", "a value", Max::Patcher::minBind(this, &Lissajous::aFunction)}
   , phaseMessage{this, "phase", "phase value", Max::Patcher::minBind(this, &Lissajous::phaseFunction)}
   , bMessage{this, "b", "b value", Max::Patcher::minBind(this, &Lissajous::bFunction)}
{
}

// see https://en.wikipedia.org/wiki/Lissajous_curve
samples<2> Lissajous::operator()(sample freq)
{
   samplerate();
   return {freq, freq};
}

atoms Lissajous::aFunction(const atoms& args, const int inlet)
{
   return {};
}

atoms Lissajous::phaseFunction(const atoms& args, const int inlet)
{
   return {};
}

atoms Lissajous::bFunction(const atoms& args, const int inlet)
{
   return {};
}

MIN_EXTERNAL(Lissajous);
