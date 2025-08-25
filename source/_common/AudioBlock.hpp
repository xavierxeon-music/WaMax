#ifndef AudioBlockHPP
#define AudioBlockHPP

#include "AudioBlock.h"

inline AudioBlock::List::~List()
{
   for (AudioBlock* block : *this)
   {
      delete block;
   }
   clear();
}

inline AudioBlock::AudioBlock(const QString& name, int counter, bool readOnly)
   : sharedMemory("SharedAudioStream_block_" + name + "_" + QString::number(counter))
   , readOnly(readOnly)
   , sharedData(nullptr)
   , cursor(0)
   , errorString()
{
   const QSharedMemory::AccessMode mode = readOnly ? QSharedMemory::ReadOnly : QSharedMemory::ReadWrite;
   if (sharedMemory.create(sizeof(Data), mode))
   {
      sharedData = reinterpret_cast<Data*>(sharedMemory.data());
      return;
   }

   errorString = sharedMemory.errorString();
   if (QSharedMemory::AlreadyExists == sharedMemory.error())
   {
      sharedMemory.attach(mode);
      if (0 == sharedMemory.size())
      {
         sharedMemory.detach();
         sharedMemory.create(sizeof(Data), mode);
      }
      errorString = sharedMemory.errorString();
      sharedData = reinterpret_cast<Data*>(sharedMemory.data());
   }
}

inline AudioBlock::~AudioBlock()
{
   if (!readOnly)
   {
      sharedMemory.lock();
      std::memset(sharedData->data, 0, BlockSize * sizeof(double));
      sharedMemory.unlock();
   }
   sharedMemory.detach();
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
