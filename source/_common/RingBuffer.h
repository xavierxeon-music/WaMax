#ifndef RingBufferH
#define RingBufferH

#include <cinttypes>
#include <vector>

template <typename DataType>
class RingBuffer
{
public:
   RingBuffer(uint16_t length);

public:
   void add(const DataType& value);
   DataType value(const uint16_t relIndex) const;
   DataType current() const;

   DataType absValue(const uint16_t absIndex) const;
   uint16_t absoluteIndex(const uint16_t relIndex) const;

   const uint16_t& getLength() const;
   const std::vector<DataType>& getBuffer() const;
   const uint16_t& getCurrentIndex() const;

private:
   uint16_t length;
   std::vector<DataType> buffer;
   uint16_t currentIndex;
};

#ifndef RingBufferHPP
#include "RingBuffer.hpp"
#endif // NOT RingBufferHPP

#endif // NOT RingBufferH
