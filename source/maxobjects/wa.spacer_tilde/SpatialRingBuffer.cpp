#include "SpatialRingBuffer.h"

#include <MathGeneral.h>

Spatial::RingBuffer::RingBuffer()
   : buffer{}
   , currentIndex(bufferSize)
   , currentCoords{}
   , currentFunction()
   , targetCoords{}
   , lastValue(0.0)
{
}

void Spatial::RingBuffer::add(const double& value, const Math::Spherical& coords)
{
   currentIndex++;
   if (currentIndex >= bufferSize)
      currentIndex = 0;

   targetCoords = coords;

   if (Math::signChange(lastValue, value))
   {
      currentCoords = targetCoords;
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
      const auto [leftAmplitude, rightAmplitude] = buffer[counter].function.value(index);

      const double& inValue = buffer[counter].value;
      left += leftAmplitude * inValue;
      right += rightAmplitude * inValue;
   }
   left /= 16.0;
   right /= 16.0;

   return std::make_tuple(left, right);
}

uint16_t Spatial::RingBuffer::relativeIndex(const uint16_t counter) const
{
   uint16_t index = (counter + bufferSize - currentIndex) % bufferSize;
   return index;
}
