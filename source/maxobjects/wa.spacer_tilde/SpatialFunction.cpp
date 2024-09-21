#include "SpatialFunction.h"

#include <MathVector3.h>
#include <Tools.h>

#include <cmath>

Spatial::Function::Function(const Math::Spherical& coords, bool left)
   : max(0.0)
   , peak(0.0)
   , start(0.0)
   , end(0.0)
{
   static const Tools::Range maxClamp(0.1, 0.8);
   static const Tools::Mapper valueClamp(Tools::Range(-1.0, 1.0), Tools::Range(0.0, 1.0));
   static const Tools::Mapper startClamp(Tools::Range(0, 1.0), Tools::Range(1.0, 3.0));
   static const Tools::Mapper endClamp(Tools::Range(0, 1.0), Tools::Range(5.0, 10.0));
   static const double earWeight = 0.65;

   static const Math::Vector3 up(0, 0, 1);
   static const Math::Vector3 forward(1, 0, 0);

   const Math::Vector3 ear = left ? Math::Vector3(0, -1, 0) : Math::Vector3(0, 1, 0);
   const Math::Vector3 dir = Math::Vector3::fromSpherical(coords);
   const Math::Vector3 normDir = dir.norm();

   const double upNess = normDir.dot(up);
   const double forwardNess = normDir.dot(forward);

   double value = (earWeight * normDir.dot(ear)) + ((1.0 - earWeight) * upNess);
   value = valueClamp(value);

   max = maxClamp(value);
   peak = 20 + (30 * (1.0 - value));

   start = peak - startClamp(value);
   end = peak - endClamp(value);
}

Spatial::Function::Function(const double& max, const double& peak, const double& start, const double& end)
   : max(max)
   , peak(peak)
   , start(start)
   , end(end)
{
}

double Spatial::Function::value(int index) const
{
   const double halfwidth = (index < peak) ? (peak - start) : (end - peak);

   const double raise = (index - peak) / (2 * halfwidth);
   const double y = max * std::exp(-raise * raise);

   return y;
}
