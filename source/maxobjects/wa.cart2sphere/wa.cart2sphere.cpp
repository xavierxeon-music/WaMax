#include "wa.cart2sphere.h"

#include <patcher.h>

Cart2Sphere::Cart2Sphere(const atoms& args)
   : object<Cart2Sphere>()
   , cartesian()
   , output{this, "(int) integer value"}
   , asDegrees{this, "asDegrees", true}
   , xMessage{this, "x", "x value", Patcher::minBind(this, &Cart2Sphere::xFunction)}
   , yMessage{this, "y", "y value", Patcher::minBind(this, &Cart2Sphere::xFunction)}
   , zMessage{this, "z", "z value", Patcher::minBind(this, &Cart2Sphere::xFunction)}
   , listMessage{this, "list", Patcher::minBind(this, &Cart2Sphere::listFunction)}
   , bangMessage{this, "bang", Patcher::minBind(this, &Cart2Sphere::calculateFunction)}
   , doubleClick{this, "dblclick", Patcher::minBind(this, &Cart2Sphere::calculateFunction)}
{
   listFunction(args, 0);
}

atoms Cart2Sphere::xFunction(const atoms& args, const int inlet)
{
   cartesian.setA(args[0]);
   calcluate();

   return {};
}

atoms Cart2Sphere::yFunction(const atoms& args, const int inlet)
{
   cartesian.setB(args[0]);
   calcluate();

   return {};
}

atoms Cart2Sphere::zFunction(const atoms& args, const int inlet)
{
   cartesian.setC(args[0]);
   calcluate();

   return {};
}

atoms Cart2Sphere::listFunction(const atoms& args, const int inlet)
{
   if (args.size() > 0)
      cartesian.setA(args[0]);
   if (args.size() > 1)
      cartesian.setB(args[1]);
   if (args.size() > 2)
      cartesian.setC(args[2]);

   calcluate();

   return {};
}

atoms Cart2Sphere::calculateFunction(const atoms& args, const int inlet)
{
   calcluate();

   return {};
}

void Cart2Sphere::calcluate()
{
   const Linalg::Vector3 spherical = cartesian.cart2Sphre(asDegrees);

   atoms result = {spherical.getA(), spherical.getB(), spherical.getC()};
   output.send(result);
}

MIN_EXTERNAL(Cart2Sphere);
