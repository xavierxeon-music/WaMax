#include "wa.coord.cart2sphere.h"

#include <MaxPatcher.h>

Coord::Cart2Sphere::Cart2Sphere(const atoms& args)
   : object<Cart2Sphere>()
   , cartesian()
   , output{this, "(int) integer value"}
   , asDegrees{this, "asDegrees", true}
   , xMessage{this, "x", "x value", Max::Patcher::minBind(this, &Cart2Sphere::xFunction)}
   , yMessage{this, "y", "y value", Max::Patcher::minBind(this, &Cart2Sphere::yFunction)}
   , zMessage{this, "z", "z value", Max::Patcher::minBind(this, &Cart2Sphere::zFunction)}
   , listMessage{this, "list", Max::Patcher::minBind(this, &Cart2Sphere::listFunction)}
   , bangMessage{this, "bang", Max::Patcher::minBind(this, &Cart2Sphere::calculateFunction)}
   , doubleClick{this, "dblclick", Max::Patcher::minBind(this, &Cart2Sphere::calculateFunction)}
{
   listFunction(args, 0);
}

atoms Coord::Cart2Sphere::xFunction(const atoms& args, const int inlet)
{
   cartesian[0] = args[0];
   calculate();

   return {};
}

atoms Coord::Cart2Sphere::yFunction(const atoms& args, const int inlet)
{
   cartesian[1] = args[0];
   calculate();

   return {};
}

atoms Coord::Cart2Sphere::zFunction(const atoms& args, const int inlet)
{
   cartesian[2] = args[0];
   calculate();

   return {};
}

atoms Coord::Cart2Sphere::listFunction(const atoms& args, const int inlet)
{
   if (args.size() > 0)
      cartesian[0] = args[0];
   if (args.size() > 1)
      cartesian[1] = args[1];
   if (args.size() > 2)
      cartesian[2] = args[2];

   calculate();

   return {};
}

atoms Coord::Cart2Sphere::calculateFunction(const atoms& args, const int inlet)
{
   calculate();

   return {};
}

void Coord::Cart2Sphere::calculate()
{
   const XX::Linalg::Spherical spherical = cartesian.toSpherical(asDegrees);

   atoms result = {spherical.az, spherical.el, spherical.radius};
   output.send(result);
}

MIN_EXTERNAL(Coord::Cart2Sphere);
