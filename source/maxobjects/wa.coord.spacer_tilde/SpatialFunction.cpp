#include "SpatialFunction.h"

#include <Tools.h>

#include <cmath>

// param

Spatial::Function::Param::Param(const double& max, const double& peak, const double& start, const double& end)
   : max(max)
   , peak(peak)
   , start(start)
   , end(end)
{
}

// function

Spatial::Function::Function(const Math::Spherical& coords)
   : cache{}
   , left()
   , right()
{
   static const Tools::Mapper valueClamp(Tools::Range(-1.0, 1.0), Tools::Range(0.0, 1.0));
   static const double earWeight = 0.65;

   const Math::Vector3 dir = Math::Vector3::fromSpherical(coords);
   const Math::Vector3 normDir = dir.norm();

   const double peakDistance = 15.0;

   auto createParam = [&](bool invertValue)
   {
      Param param;

      // max
      param.max = earWeight * normDir.getY();
      if (invertValue)
         param.max *= -1.0;
      param.max += (1.0 - earWeight) * normDir.getZ();
      param.max = valueClamp(param.max);
      if (normDir.getX() > 0.0)
         param.max += 0.3 * std::pow(normDir.getX(), 2);

      // start
      param.start = 14.0 * -normDir.getY();
      if (invertValue)
         param.start *= -1.0;
      param.start = 16.0 + param.start;

      // peak
      param.peak = param.start + 2.0;

      // end
      param.end = param.start + 10.0;
      auto getBulge = [&]()
      {
         if (invertValue)
         {
            if (normDir.getY() > 0)
               return std::pow(normDir.getY(), 2);
         }
         else
         {
            if (normDir.getY() < 0)
               return std::pow(normDir.getY(), 2);
         }

         return 0.0;
      };

      static const double bulgeScale = 4.0;
      param.end += bulgeScale * getBulge();

      return param;
   };

   left = createParam(false);
   right = createParam(true);

   fillCache();
}

Spatial::Function::Function(const Param& left, const Param& right)
   : cache{}
   , left(left)
   , right(right)
{
   fillCache();
}

const Spatial::Stereo& Spatial::Function::value(int index) const
{
   return cache[index];
}

void Spatial::Function::shiftCache(int steps)
{
   auto moveParam = [&](Param& param)
   {
      param.start += steps;
      param.peak += steps;
      param.end += steps;
   };

   moveParam(left);
   moveParam(right);

   fillCache();
}

const Spatial::Function::Param& Spatial::Function::getLeftParam() const
{
   return left;
}

const Spatial::Function::Param& Spatial::Function::getRightParam() const
{
   return right;
}

void Spatial::Function::fillCache()
{
   for (int index = 0; index < bufferSize; index++)
   {
      auto getValue = [&](const Param& param)
      {
         const double halfwidth = (index < param.peak) ? (param.peak - param.start) : (param.end - param.peak);
         const double raise = (index - param.peak) / (2 * halfwidth);
         const double value = param.max * std::exp(-raise * raise);

         return value;
      };

      const double leftValue = getValue(left);
      const double rightValue = getValue(right);

      const Stereo stereo = Stereo{leftValue, rightValue};
      cache[index] = stereo;
   }
}
