#ifndef SpatialFunctionH
#define SpatialFunctionH

#include "SpatialCoords.h"

namespace Spatial
{
   class Function
   {
   public:
      Function(const Coords& coords, bool left);
      Function(const double& max = 0.0, const double& peak = 0.0, const double& start = 0.0, const double& end = 0.0);

   public:
      double value(int index) const;

   public:
      double max;
      double peak;
      double start;
      double end;
   };
} // namespace Spatial

#endif // NOT SpatialFunctionH
