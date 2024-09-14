#include "wa.cart2sphere.h"

#include <patcher.h>

Cart2Sphere::Cart2Sphere(const atoms& args)
   : object<Cart2Sphere>()
   , cartesian()
   , input{this, "set values"}
   , output{this, "(int) integer value"}
   , xValue{this, "x", "x value", Patcher::minBind(this, &Cart2Sphere::xFunction)}
   , yValue{this, "y", "y value", Patcher::minBind(this, &Cart2Sphere::xFunction)}
   , zValue{this, "z", "z value", Patcher::minBind(this, &Cart2Sphere::xFunction)}
   , doubleClick{this, "dblclick", Patcher::minBind(this, &Cart2Sphere::doubleClickedFunction)}
{
   if (args.size() > 0)
      cartesian.setA(args[0]);
   if (args.size() > 1)
      cartesian.setB(args[1]);
   if (args.size() > 2)
      cartesian.setC(args[2]);

   calcluate();
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

atoms Cart2Sphere::doubleClickedFunction(const atoms& args, const int inlet)
{
   calcluate();

   return {};
}

void Cart2Sphere::calcluate()
{
   cout << cartesian.getA() << " " << cartesian.getB() << " " << cartesian.getC() << endl;
}

MIN_EXTERNAL(Cart2Sphere);
