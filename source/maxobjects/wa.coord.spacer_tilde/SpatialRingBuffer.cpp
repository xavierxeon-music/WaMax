#include "SpatialRingBuffer.h"

#include <Math/MathGeneral.h>

Spatial::RingBuffer::RingBuffer()
   : buffer{}
   , currentIndex(Function::length)
   , currentCoords{}
   , currentFunction()
   , lastValue(0.0)
{
}

void Spatial::RingBuffer::add(const double& value, const Math::Spherical& coords)
{
   currentIndex++;
   if (currentIndex >= Function::length)
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
   Stereo signal;
   Stereo totalAmplitude;

   for (int16_t counter = 0; counter < Function::length; counter++)
   {
      int16_t index = relativeIndex(counter);
      const Stereo& amplitude = buffer[counter].function.value(index);
      totalAmplitude += amplitude;

      const double& sourceValue = buffer[counter].value;
      signal += (amplitude * sourceValue);
   }

   const double max = totalAmplitude.max();
   signal *= (1.0 / max);
   return signal;
}

int16_t Spatial::RingBuffer::relativeIndex(const int16_t counter) const
{
   int16_t index = (counter + Function::length - currentIndex) % Function::length;
   return index;
}
