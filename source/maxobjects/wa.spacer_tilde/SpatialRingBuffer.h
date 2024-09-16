#ifndef SpatialRingBufferH
#define SpatialRingBufferH

#include <cinttypes>

namespace Spatial
{
   class RingBuffer
   {
   public:
      struct Entry
      {
         double value = 0;
         double az = 0.0;
         double el = 0.0;
      };

   public:
      RingBuffer();

   public:
      void add(const double& value, const double& az, const double& el);
      double convolve(bool left) const;
      uint16_t relativeIndex(const uint16_t counter) const;

   private:
      static constexpr uint16_t bufferSize = 128;
      Entry buffer[bufferSize];
      uint16_t current;
   };
} // namespace Spatial

#endif // NOT SpatialRingBufferH
