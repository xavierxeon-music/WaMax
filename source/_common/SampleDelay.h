#ifndef SampleDelayH
#define SampleDelayH

#include <cinttypes>
#include <vector>

#include "c74_min.h"
using namespace c74::min;

class SampleDelay
{
public:
   using List = std::vector<SampleDelay>;

public:
   SampleDelay(uint16_t length = 0);

public:
   void tapin(const sample& value, bool absValue = false);
   sample tapout(const uint16_t counter) const;
   sample sum() const;
   sample average() const;

   uint16_t relativeIndex(const uint16_t counter) const;

private:
   uint16_t length;
   std::vector<sample> buffer;
   uint16_t currentIndex;
   sample currentSum;
};

#ifndef SampleDelayHPP
#include "SampleDelay.hpp"
#endif // NOT SampleDelayHPP

#endif // NOT SampleDelayH
