#ifndef SampleDelayHPP
#define SampleDelayHPP

#include "SampleDelay.h"

inline SampleDelay::SampleDelay(uint16_t length)
   : RingBuffer<sample>(length)
{
}

inline void SampleDelay::tapin(const sample& value, bool absValue)
{
   if (absValue && value < 0.0)
      add(-value);
   else
      add(value);
}

inline sample SampleDelay::tapout(const uint16_t counter) const
{
   return value(counter);
}

inline sample SampleDelay::sum() const
{
   sample sumValue = 0.0;
   for (const sample& value : getBuffer())
   {
      sumValue += value;
   }

   return sumValue;
}

inline sample SampleDelay::average() const
{
   return sum() / getLength();
}

inline sample SampleDelay::peak() const
{
   sample peakValue = 0.0;
   for (const sample& value : getBuffer())
   {
      if (value > peakValue)
         peakValue = value;
   }

   return peakValue;
}

#endif // NOT SampleDelayHPP