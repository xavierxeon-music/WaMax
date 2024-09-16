#include "SpatialRingBuffer.h"

#include "SpatialFunction.h"

Spatial::RingBuffer::RingBuffer()
   : buffer{}
   , current(bufferSize)
{
}

void Spatial::RingBuffer::add(const double& value, const double& az, const double& el)
{
   current++;
   if (current >= bufferSize)
      current = 0;

   buffer[current] = Entry{value, az, el};
}

double Spatial::RingBuffer::convolve(bool left) const
{
   double out = 0;

   for (uint16_t counter = 0; counter < bufferSize; counter++)
   {
      const double& inValue = buffer[counter].value;
      Function f(buffer[counter].az, buffer[counter].el, left);

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
   uint16_t index = (counter + bufferSize - current) % bufferSize;
   return index;
}
