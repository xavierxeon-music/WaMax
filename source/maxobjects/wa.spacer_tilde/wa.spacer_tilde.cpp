#include "wa.spacer_tilde.h"

#include <MaxPatcher.h>

#include "SpatialFunction.h"

Spacer::Spacer(const atoms& args)
   : object<Spacer>()
   , sample_operator<1, 2>()
   , cartesian()
   , buffer()
   , input{this, "(signal) signal"}
   , leftOutput{this, "(signal) left signal", "signal"}
   , rightOutput{this, "(signal) right signal", "signal"}
   , asDegrees{this, "asDegrees", true}
   , xMessage{this, "x", "x value", Max::Patcher::minBind(this, &Spacer::xFunction)}
   , yMessage{this, "y", "y value", Max::Patcher::minBind(this, &Spacer::yFunction)}
   , zMessage{this, "z", "z value", Max::Patcher::minBind(this, &Spacer::zFunction)}
   , listMessage{this, "list", Max::Patcher::minBind(this, &Spacer::listFunction)}
{
   listFunction(args, 0);
}

samples<2> Spacer::operator()(sample in)
{
   buffer.add(in, spherical);
   sample left = buffer.convolve(true);
   sample right = buffer.convolve(false);

   return {left, right};
}

atoms Spacer::xFunction(const atoms& args, const int inlet)
{
   cartesian[0] = args[0];
   spherical = cartesian.toSpherical(asDegrees);

   return {};
}

atoms Spacer::yFunction(const atoms& args, const int inlet)
{
   cartesian[1] = args[0];
   spherical = cartesian.toSpherical(asDegrees);

   return {};
}

atoms Spacer::zFunction(const atoms& args, const int inlet)
{
   cartesian[2] = args[0];
   spherical = cartesian.toSpherical(asDegrees);

   return {};
}

atoms Spacer::listFunction(const atoms& args, const int inlet)
{
   if (args.size() > 0)
      cartesian[0] = args[0];
   if (args.size() > 1)
      cartesian[1] = args[1];
   if (args.size() > 2)
      cartesian[2] = args[2];

   spherical = cartesian.toSpherical(asDegrees);

   return {};
}

MIN_EXTERNAL(Spacer);
