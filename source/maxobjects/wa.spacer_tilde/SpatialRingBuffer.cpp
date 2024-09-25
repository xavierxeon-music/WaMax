#include "SpatialRingBuffer.h"

#include <MathGeneral.h>

Spatial::RingBuffer::RingBuffer()
   : buffer{}
   , currentIndex(bufferSize)
   , currentCoords{}
   , currentFunction()
   , lastValue(0.0)
{
}

void Spatial::RingBuffer::add(const double& value, const Math::Spherical& coords)
{
   currentIndex++;
   if (currentIndex >= bufferSize)
      currentIndex = 0;

   if (Math::signChange(lastValue, value))
   {
      currentCoords = coords;
      currentFunction = Function(currentCoords);
   }

   lastValue = value;

   buffer[currentIndex] = Entry{value, currentFunction};
}

Spatial::Stereo Spatial::RingBuffer::convolve() const
{
   double left = 0;
   double right = 0;
   for (uint16_t counter = 0; counter < bufferSize; counter++)
   {
      uint16_t index = relativeIndex(counter);
      const Stereo& amplitude = buffer[counter].function.value(index);

      const double& inValue = buffer[counter].value;
      left += amplitude.left * inValue;
      right += amplitude.right * inValue;
   }
   left /= 16.0;
   right /= 16.0;

   return Stereo{left, right};
}

uint16_t Spatial::RingBuffer::relativeIndex(const uint16_t counter) const
{
   uint16_t index = (counter + bufferSize - currentIndex) % bufferSize;
   return index;
}
