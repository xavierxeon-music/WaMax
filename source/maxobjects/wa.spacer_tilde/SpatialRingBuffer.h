#ifndef SpatialRingBufferH
#define SpatialRingBufferH

#include <cinttypes>

#include "SpatialCoords.h"

namespace Spatial
{
   class RingBuffer
   {
   public:
      struct Entry
      {
         double value = 0;
         Coords coords;
      };

   public:
      RingBuffer();

   public:
      void add(const double& value, const Coords& coords);
      double convolve(bool left) const;
      uint16_t relativeIndex(const uint16_t counter) const;

   private:
      static constexpr uint16_t bufferSize = 128;
      Entry buffer[bufferSize];
      uint16_t currentIndex;

      Spatial::Coords currentCoords;
      Spatial::Coords targetCoords;
      double lastValue;
   };
} // namespace Spatial

#endif // NOT SpatialRingBufferH
