#include "wa.coord.spacer_tilde.h"

#include <MaxPatcher.h>

#include "SpatialFunction.h"

Coord::Spacer::Spacer(const atoms& args)
   : object<Spacer>()
   , sample_operator<1, 2>()
   , cartesian()
   , buffer()
   , input{this, "(signal) signal"}
   , leftOutput{this, "(signal) left signal", "signal"}
   , rightOutput{this, "(signal) right signal", "signal"}
   , asDegrees{this, "asDegrees", true}
   , active{this, "active", true}
   , xMessage{this, "x", "x value", Max::Patcher::minBind(this, &Spacer::xFunction)}
   , yMessage{this, "y", "y value", Max::Patcher::minBind(this, &Spacer::yFunction)}
   , zMessage{this, "z", "z value", Max::Patcher::minBind(this, &Spacer::zFunction)}
   , listMessage{this, "list", Max::Patcher::minBind(this, &Spacer::listFunction)}
{
   listFunction(args, 0);
}

samples<2> Coord::Spacer::operator()(sample in)
{
   if (!active)
      return {in, in};

   buffer.add(in, spherical);

   const Spatial::Stereo stereo = buffer.convolve();
   return {stereo.left, stereo.right};
}

atoms Coord::Spacer::xFunction(const atoms& args, const int inlet)
{
   const double value = args[0];
   if (cartesian[0] != value)
   {
      cartesian[0] = value;
      spherical = cartesian.toSpherical(asDegrees);
   }

   return {};
}

atoms Coord::Spacer::yFunction(const atoms& args, const int inlet)
{
   const double value = args[0];
   if (cartesian[1] != value)
   {
      cartesian[1] = value;
      spherical = cartesian.toSpherical(asDegrees);
   }

   return {};
}

atoms Coord::Spacer::zFunction(const atoms& args, const int inlet)
{
   const double value = args[0];
   if (cartesian[2] != value)
   {
      cartesian[2] = value;
      spherical = cartesian.toSpherical(asDegrees);
   }

   return {};
}

atoms Coord::Spacer::listFunction(const atoms& args, const int inlet)
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

MIN_EXTERNAL(Coord::Spacer);
