#include "SpatialRingBuffer.h"

#include <thread>

#include <XXMath.h>

Spatial::RingBuffer::RingBuffer()
   : buffer{}
   , currentIndex(Function::length)
   , currentCoords{}
   , currentFunction()
   , lastValue(0.0)
{
}

void Spatial::RingBuffer::add(const double& value, const XX::Linalg::Spherical& coords)
{
   currentIndex++;
   if (currentIndex >= Function::length)
      currentIndex = 0;

   if (XX::Math::signChange(lastValue, value))
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
   Stereo accumulatedAmplitude;

   for (uint8_t counter = 0; counter < Function::length; counter++)
   {
      const Entry& entry = buffer[counter];
      const uint8_t index = relativeIndex(counter);

      const Stereo& amplitude = entry.function.value(index);
      accumulatedAmplitude += amplitude;
      signal += (amplitude * entry.value);
   }

   const double max = accumulatedAmplitude.max();
   signal *= (1.0 / max);
   return signal;
}

uint8_t Spatial::RingBuffer::relativeIndex(const uint8_t counter) const
{
   uint8_t index = (counter + Function::length - currentIndex) % Function::length;
   return index;
}
