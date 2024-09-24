#include "SpatialFunction.h"

#include <MathVector3.h>
#include <Tools.h>

#include <cmath>

Spatial::Function::Param::Param(const double& max, const double& peak, const double& start, const double& end)
   : max(max)
   , peak(peak)
   , start(start)
   , end(end)
{
}

Spatial::Function::Function(const Math::Spherical& coords)
   : left()
   , right()
{
   static const Tools::Range maxClamp(0.1, 0.8);
   static const Tools::Mapper valueClamp(Tools::Range(-1.0, 1.0), Tools::Range(0.0, 1.0));
   static const Tools::Mapper startClamp(Tools::Range(0, 1.0), Tools::Range(1.0, 3.0));
   static const Tools::Mapper endClamp(Tools::Range(0, 1.0), Tools::Range(5.0, 10.0));
   static const Tools::Mapper forwardClamp(Tools::Range(-1.0, 1.0), Tools::Range(0.0, 1.0));
   static const double earWeight = 0.65;

   const Math::Vector3 dir = Math::Vector3::fromSpherical(coords);
   const Math::Vector3 normDir = dir.norm();

   auto fillParam = [&](Param& param, bool invertValue)
   {
      double value = earWeight * normDir.getY();
      if (invertValue)
         value *= -1.0;
      value += (1.0 - earWeight) * normDir.getZ();
      value = valueClamp(value);

      param.max = maxClamp(value);
      param.peak = 20 + (30 * (1.0 - value));

      param.start = param.peak - startClamp(value) - forwardClamp(normDir.getX());
      param.end = param.peak - endClamp(value);
   };

   fillParam(left, false);
   fillParam(right, true);
}

Spatial::Function::Function(const Param& left, const Param& right)
   : left(left)
   , right(right)
{
}

Spatial::Stereo Spatial::Function::value(int index) const
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

   return std::make_tuple(leftValue, rightValue);
}
