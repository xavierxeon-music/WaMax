#ifndef SpatialRingBufferH
#define SpatialRingBufferH

#include <MathVector3.h>

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
      uint8_t relativeIndex(const uint8_t counter) const;

   private:
      Entry buffer[Function::length];
      uint8_t currentIndex;

      Math::Spherical currentCoords;
      Function currentFunction;

      double lastValue;
   };
} // namespace Spatial

#endif // NOT SpatialRingBufferH
