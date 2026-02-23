#ifndef SpatialFunctionH
#define SpatialFunctionH

#include <XXMathVector3.h>

#include "SpatialStereo.h"

namespace Spatial
{

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

      static constexpr uint8_t length = 64;

   public:
      Function(const XX::Math::Spherical& coords);
      Function(const Param& left = Param(), const Param& right = Param());

   public:
      const Stereo& value(int index) const;
      void shiftCache(int steps);
      const Param& getLeftParam() const;
      const Param& getRightParam() const;

   private:
      void fillCache();

   private:
      Stereo cache[length];
      Param left;
      Param right;
   };
} // namespace Spatial

#endif // NOT SpatialFunctionH
