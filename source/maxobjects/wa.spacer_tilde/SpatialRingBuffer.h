#ifndef SpatialRingBufferH
#define SpatialRingBufferH

#include <cinttypes>

#include "MathVector3.h"
#include "SpatialFunction.h"

namespace Spatial
{
   class RingBuffer
   {
   public:
      struct Entry
      {
         double value = 0;
         Math::Spherical coords;
         Function leftFunction;
         Function rightFunction;
      };

   public:
      RingBuffer();

   public:
      void add(const double& value, const Math::Spherical& coords);
      std::tuple<double, double> convolve() const;
      uint16_t relativeIndex(const uint16_t counter) const;

   private:
      static constexpr uint16_t bufferSize = 80;
      Entry buffer[bufferSize];
      uint16_t currentIndex;

      Math::Spherical currentCoords;
      Function currentLeftFunction;
      Function currentRightFunction;

      Math::Spherical targetCoords;
      double lastValue;
   };
} // namespace Spatial

#endif // NOT SpatialRingBufferH
