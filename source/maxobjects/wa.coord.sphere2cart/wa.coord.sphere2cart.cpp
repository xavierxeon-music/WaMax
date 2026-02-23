#include "wa.coord.sphere2cart.h"

#include <MaxPatcher.h>

Coord::Sphere2Cart::Sphere2Cart(const atoms& args)
   : object<Sphere2Cart>()
   , spherical()
   , output{this, "(int) integer value"}
   , asDegrees{this, "asDegrees", true}
   , azMessage{this, "az", "azimuth value", Max::Patcher::minBind(this, &Sphere2Cart::azFunction)}
   , elMessage{this, "el", "elevation value", Max::Patcher::minBind(this, &Sphere2Cart::elFunction)}
   , radiusMessage{this, "radius", "radius value", Max::Patcher::minBind(this, &Sphere2Cart::radiusFunction)}
   , listMessage{this, "list", Max::Patcher::minBind(this, &Sphere2Cart::listFunction)}
   , bangMessage{this, "bang", Max::Patcher::minBind(this, &Sphere2Cart::calculateFunction)}
   , doubleClick{this, "dblclick", Max::Patcher::minBind(this, &Sphere2Cart::calculateFunction)}
{
   listFunction(args, 0);
}

atoms Coord::Sphere2Cart::azFunction(const atoms& args, const int inlet)
{
   spherical.az = args[0];
   calculate();

   return {};
}

atoms Coord::Sphere2Cart::elFunction(const atoms& args, const int inlet)
{
   spherical.el = args[0];
   calculate();

   return {};
}

atoms Coord::Sphere2Cart::radiusFunction(const atoms& args, const int inlet)
{
   spherical.radius = args[0];
   calculate();

   return {};
}

atoms Coord::Sphere2Cart::listFunction(const atoms& args, const int inlet)
{
   if (args.size() > 0)
      spherical.az = args[0];
   if (args.size() > 1)
      spherical.el = args[1];
   if (args.size() > 2)
      spherical.radius = args[2];

   calculate();

   return {};
}

atoms Coord::Sphere2Cart::calculateFunction(const atoms& args, const int inlet)
{
   calculate();

   return {};
}

void Coord::Sphere2Cart::calculate()
{
   const XX::Math::Vector3 cartesian = XX::Math::Vector3::fromSpherical(spherical, asDegrees);

   atoms result = {cartesian[0], cartesian[1], cartesian[2]};
   output.send(result);
}

MIN_EXTERNAL(Coord::Sphere2Cart);
