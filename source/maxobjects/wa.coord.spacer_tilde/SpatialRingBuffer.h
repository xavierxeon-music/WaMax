#ifndef SpatialRingBufferH
#define SpatialRingBufferH

#include <XXMathVector3.h>

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
      void add(const double& value, const XX::Math::Spherical& coords);
      Stereo convolve() const;
      uint8_t relativeIndex(const uint8_t counter) const;

   private:
      Entry buffer[Function::length];
      uint8_t currentIndex;

      XX::Math::Spherical currentCoords;
      Function currentFunction;

      double lastValue;
   };
} // namespace Spatial

#endif // NOT SpatialRingBufferH
