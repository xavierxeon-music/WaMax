#ifndef SampleDelayH
#define SampleDelayH

#include <RingBuffer.h>

#include "c74_min.h"
using namespace c74::min;

class SampleDelay : public RingBuffer<sample>
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
   sample peak() const;
};

#ifndef SampleDelayHPP
#include "SampleDelay.hpp"
#endif // NOT SampleDelayHPP

#endif // NOT SampleDelayH
