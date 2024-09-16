#ifndef SpatialFunctionH
#define SpatialFunctionH

#include "SpatialCoords.h"

namespace Spatial
{
   class Function
   {
   public:
      Function(const Coords& coords, bool left);

   public:
      double value(int index) const;

   private:
      double max;
      double peak;
      double start;
      double end;
   };
} // namespace Spatial

#endif // NOT SpatialFunctionH
