#ifndef RingBufferHPP
#define RingBufferHPP

#include "RingBuffer.h"

template <typename DataType>
RingBuffer<DataType>::RingBuffer(uint16_t length)
   : length(length)
   , buffer()
   , currentIndex(length)
{
   buffer = std::vector<DataType>(length, 0.0);
}

template <typename DataType>
void RingBuffer<DataType>::add(const DataType& value)
{
   currentIndex++;
   if (currentIndex >= length)
      currentIndex = 0;

   buffer[currentIndex] = value;
}

template <typename DataType>
DataType RingBuffer<DataType>::value(const uint16_t relIndex) const
{
   const uint16_t absIndex = absoluteIndex(relIndex);
   return buffer.at(absIndex);
}

template <typename DataType>
DataType RingBuffer<DataType>::current() const
{
   return buffer.at(currentIndex);
}

template <typename DataType>
DataType RingBuffer<DataType>::absValue(const uint16_t absIndex) const
{
   return buffer.at(absIndex);
}

template <typename DataType>
uint16_t RingBuffer<DataType>::absoluteIndex(const uint16_t relIndex) const
{
   const uint16_t index = (relIndex + length - currentIndex) % length;
   return index;
}

template <typename DataType>
const uint16_t& RingBuffer<DataType>::getLength() const
{
   return length;
}

template <typename DataType>
const std::vector<DataType>& RingBuffer<DataType>::getBuffer() const
{
   return buffer;
}

template <typename DataType>
const uint16_t& RingBuffer<DataType>::getCurrentIndex() const
{
   return currentIndex;
}

#endif // NOT RingBufferHPP
