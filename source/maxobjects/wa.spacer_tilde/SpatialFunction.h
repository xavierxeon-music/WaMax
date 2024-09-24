#ifndef SpatialFunctionH
#define SpatialFunctionH

#include <tuple>

#include "MathVector3.h"

namespace Spatial
{
   using Stereo = std::tuple<double, double>;

   class Function
   {
   public:
      struct Param
      {
         double max;
         double peak;
         double start;
         double end;

         Param(const double& max = 0.0, const double& peak = 0.0, const double& start = 0.0, const double& end = 0.0);
      };

   public:
      Function(const Math::Spherical& coords);
      Function(const Param& left = Param(), const Param& right = Param());

   public:
      Stereo value(int index) const;

   public:
      Param left;
      Param right;
   };
} // namespace Spatial

#endif // NOT SpatialFunctionH
