#include "SpatialRingBuffer.h"

#include "SpatialFunction.h"

Spatial::RingBuffer::RingBuffer()
   : buffer{}
   , current(128)
{
}

void Spatial::RingBuffer::add(const double& value, const double& az, const double& el)
{
   current++;
   if (current >= 128)
      current = 0;

   buffer[current] = Entry{value, az, el};
}

double Spatial::RingBuffer::convolve(bool left) const
{
   double out = 0;

   for (uint8_t counter = 0; counter < 128; counter++)
   {
      const double& inValue = buffer[counter].value;
      Function f(buffer[counter].az, buffer[counter].el, left);

      uint8_t index = (counter + 128 - current) % 128;
      const double amplitude = f.value(counter);

      const double outValue = amplitude * inValue;
      out += outValue;
   }

   out /= 8.0;

   return out;
}