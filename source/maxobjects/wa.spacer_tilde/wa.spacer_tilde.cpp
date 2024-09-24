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
   const auto [left, right] = buffer.convolve();

   return {left, right};
}

atoms Spacer::xFunction(const atoms& args, const int inlet)
{
   const double value = args[0];
   if (cartesian[0] != value)
   {
      cartesian[0] = value;
      spherical = cartesian.toSpherical(asDegrees);
   }

   return {};
}

atoms Spacer::yFunction(const atoms& args, const int inlet)
{
   const double value = args[0];
   if (cartesian[1] != value)
   {
      cartesian[1] = value;
      spherical = cartesian.toSpherical(asDegrees);
   }

   return {};
}

atoms Spacer::zFunction(const atoms& args, const int inlet)
{
   const double value = args[0];
   if (cartesian[2] != value)
   {
      cartesian[2] = value;
      spherical = cartesian.toSpherical(asDegrees);
   }

   return {};
}

atoms Spacer::listFunction(const atoms& args, const int inlet)
{
   bool change = false;
   for (const int& index : {0, 1, 2})
   {
      if (args.size() <= index)
         break;
      const double value = args[index];
      if (cartesian[index] == value)
         continue;

      cartesian[index] = value;
      change = true;
   }

   if (change)
      spherical = cartesian.toSpherical(asDegrees);

   return {};
}

MIN_EXTERNAL(Spacer);
