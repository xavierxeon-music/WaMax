#ifndef AudioBlockHPP
#define AudioBlockHPP

#include "AudioBlock.h"

inline AudioBlock::AudioBlock(const QString& name, int counter)
   : sharedMemory("SharedAudioStream_block_" + name + "_" + QString::number(counter))
   , sharedData(nullptr)
   , cursor(0)
   , errorString()
{
   if (sharedMemory.create(sizeof(Data)))
   {
      sharedData = reinterpret_cast<Data*>(sharedMemory.data());
      return;
   }

   errorString = sharedMemory.errorString();
   if (QSharedMemory::AlreadyExists == sharedMemory.error())
   {
      sharedMemory.attach();
      if (0 == sharedMemory.size())
      {
         sharedMemory.detach();
         sharedMemory.create(sizeof(Data));
      }
      errorString = sharedMemory.errorString();
      sharedData = reinterpret_cast<Data*>(sharedMemory.data());
   }
}

inline void AudioBlock::copyFrom(const double* data, const size_t& size)
{
   if (sharedData)
   {
      sharedMemory.lock();
      std::memcpy(sharedData->data + cursor, data, size * sizeof(double));
      sharedMemory.unlock();
   }

   cursor = (cursor + size) % BlockSize;
}

inline void AudioBlock::copyTo(double* data, const size_t& size)
{
   if (sharedData)
   {
      sharedMemory.lock();
      std::memcpy(data, sharedData->data + cursor, size * sizeof(double));
      sharedMemory.unlock();
   }

   cursor = (cursor + size) % BlockSize;
}

const QString& AudioBlock::getErrorString() const
{
   return errorString;
}

#endif // NOT AudioBlockHPP
