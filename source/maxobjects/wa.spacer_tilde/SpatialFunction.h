#ifndef SpatialFunctionH
#define SpatialFunctionH

#include "MathVector3.h"

namespace Spatial
{
   enum class Ear
   {
      Left,
      Right
   };

   struct Stereo
   {
      double left;
      double right;
   };

   static constexpr int16_t bufferSize = 128;

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
      const Stereo& value(int index) const;
      void shiftCache(int steps);

   private:
      void fillCache(const Param& left, const Param& right);

   private:
      Stereo cache[bufferSize];
   };
} // namespace Spatial

#endif // NOT SpatialFunctionH
