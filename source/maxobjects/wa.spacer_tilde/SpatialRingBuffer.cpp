#include "SpatialRingBuffer.h"

#include "SpatialFunction.h"
#include <MathGeneral.h>

Spatial::RingBuffer::RingBuffer()
   : buffer{}
   , currentIndex(bufferSize)
   , currentCoords{}
   , targetCoords{}
   , lastValue(0.0)
{
}

void Spatial::RingBuffer::add(const double& value, const Coords& coords)
{
   currentIndex++;
   if (currentIndex >= bufferSize)
      currentIndex = 0;

   targetCoords = coords;

   if (Math::signChange(lastValue, value))
      currentCoords = targetCoords;

   lastValue = value;

   buffer[currentIndex] = Entry{value, currentCoords};
}

double Spatial::RingBuffer::convolve(bool left) const
{
   double out = 0;

   for (uint16_t counter = 0; counter < bufferSize; counter++)
   {
      const double& inValue = buffer[counter].value;
      Function f(buffer[counter].coords, left);

      uint16_t index = relativeIndex(counter);
      const double amplitude = f.value(index);

      const double outValue = amplitude * inValue;
      out += outValue;
   }

   out /= 16.0;

   return out;
}

uint16_t Spatial::RingBuffer::relativeIndex(const uint16_t counter) const
{
   uint16_t index = (counter + bufferSize - currentIndex) % bufferSize;
   return index;
}
