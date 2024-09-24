#ifndef SpatialRingBufferH
#define SpatialRingBufferH

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
         Function function;
      };

   public:
      RingBuffer();

   public:
      void add(const double& value, const Math::Spherical& coords);
      Stereo convolve() const;
      uint16_t relativeIndex(const uint16_t counter) const;

   private:
      static constexpr uint16_t bufferSize = 128;
      Entry buffer[bufferSize];
      uint16_t currentIndex;

      Math::Spherical currentCoords;
      Function currentFunction;

      Math::Spherical targetCoords;
      double lastValue;
   };
} // namespace Spatial

#endif // NOT SpatialRingBufferH
