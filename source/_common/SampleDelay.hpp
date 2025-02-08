#ifndef SampleDelayHPP
#define SampleDelayHPP

#include "SampleDelay.h"

inline SampleDelay::SampleDelay(uint16_t length)
   : length(length)
   , buffer()
   , currentIndex(length)
   , currentSum(0)
{
   buffer = std::vector<sample>(length, 0.0);
}

inline void SampleDelay::tapin(const sample& value, bool absValue)
{
   currentIndex++;
   if (currentIndex >= length)
      currentIndex = 0;

   currentSum -= buffer[currentIndex];
   if (absValue && value < 0.0)
   {
      currentSum -= value;
      buffer[currentIndex] = -value;
   }
   else
   {
      currentSum += value;
      buffer[currentIndex] = value;
   }
}

inline sample SampleDelay::tapout(const uint16_t counter) const
{
   const uint16_t index = relativeIndex(counter);
   return buffer.at(index);
}

inline sample SampleDelay::sum() const
{
   return currentSum;
}

inline sample SampleDelay::average() const
{
   return currentSum / length;
}

inline uint16_t SampleDelay::relativeIndex(const uint16_t counter) const
{
   const uint16_t index = (counter + length - currentIndex) % length;
   return index;
}

#endif // NOT SampleDelayHPP