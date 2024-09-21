#ifndef SpatialRingBufferH
#define SpatialRingBufferH

#include <cinttypes>

#include "MathVector3.h"

namespace Spatial
{
   class RingBuffer
   {
   public:
      struct Entry
      {
         double value = 0;
         Math::Spherical coords;
      };

   public:
      RingBuffer();

   public:
      void add(const double& value, const Math::Spherical& coords);
      double convolve(bool left) const;
      uint16_t relativeIndex(const uint16_t counter) const;

   private:
      static constexpr uint16_t bufferSize = 128;
      Entry buffer[bufferSize];
      uint16_t currentIndex;

      Math::Spherical currentCoords;
      Math::Spherical targetCoords;
      double lastValue;
   };
} // namespace Spatial

#endif // NOT SpatialRingBufferH
