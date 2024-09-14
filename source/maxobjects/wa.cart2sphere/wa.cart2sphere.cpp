#include "wa.cart2sphere.h"

#include <patcher.h>

Cart2Sphere::Cart2Sphere()
   : object<Cart2Sphere>()
   , cartesian()
   , input{this, "(list) 7 bit list"}
   , output{this, "(int) integer value"}
   , xMessage{this, "x", "x value.", Patcher::minBind(this, &Cart2Sphere::xFunction)}
   , yMessage{this, "y", "y value.", Patcher::minBind(this, &Cart2Sphere::yFunction)}
   , zMessage{this, "z", "z value.", Patcher::minBind(this, &Cart2Sphere::zFunction)}
{
}

atoms Cart2Sphere::xFunction(const atoms& args, const int inlet)
{
   if (0 < args.size())
   {
      cartesian.setA(args[0]);
      calcluate();
   }
   return {};
}

atoms Cart2Sphere::yFunction(const atoms& args, const int inlet)
{
   if (0 < args.size())
   {
      cartesian.setB(args[0]);
      calcluate();
   }

   return {};
}

atoms Cart2Sphere::zFunction(const atoms& args, const int inlet)
{
   if (0 < args.size())
   {
      cartesian.setC(args[0]);
      calcluate();
   }
   return {};
}

void Cart2Sphere::calcluate()
{
   cout << cartesian.getA() << " " << cartesian.getB() << " " << cartesian.getC() << endl;
}

MIN_EXTERNAL(Cart2Sphere);
