#ifndef AudioBlockHPP
#define AudioBlockHPP

#include "AudioBlock.h"

inline AudioBlock::AudioBlock(const QString& name, int counter)
   : sharedMemory("SharedAudioStream_block_" + name + "_" + QString::number(counter))
   , sharedData(nullptr)
{
   if (sharedMemory.create(sizeof(Data)))
      return;

   if (QSharedMemory::AlreadyExists == sharedMemory.error())
   {
      sharedMemory.attach();
      if (0 == sharedMemory.size())
      {
         sharedMemory.detach();
         sharedMemory.create(sizeof(Data));
      }
      sharedData = reinterpret_cast<Data*>(sharedMemory.data());
   }
}

inline void AudioBlock::copyFrom(const double* data, const size_t& size)
{
   sharedMemory.lock();
   std::memcpy(sharedData->data, data, size * sizeof(double));
   sharedMemory.unlock();
}

inline void AudioBlock::copyTo(double* data, const size_t& size)
{
   sharedMemory.lock();
   std::memcpy(data, sharedData->data, size * sizeof(double));
   sharedMemory.unlock();
}

#endif // NOT AudioBlockHPP
