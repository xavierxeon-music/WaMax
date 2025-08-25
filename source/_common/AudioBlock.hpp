#ifndef AudioBlockHPP
#define AudioBlockHPP

#include "AudioBlock.h"

#include <QSharedMemory>

inline AudioBlock* AudioBlock::create(const QString& name, int counter)
{
   QSharedMemory* sharedMemory = new QSharedMemory("SharedAudioStream_block_" + name + "_" + QString::number(counter));
   if (sharedMemory->create(sizeof(AudioBlock)))
      return reinterpret_cast<AudioBlock*>(sharedMemory->data());

   if (QSharedMemory::AlreadyExists == sharedMemory->error())
   {
      sharedMemory->attach();
      if (0 == sharedMemory->size())
      {
         sharedMemory->detach();
         sharedMemory->create(sizeof(AudioBlock));
      }
      return reinterpret_cast<AudioBlock*>(sharedMemory->data());
   }

   return nullptr;
}

#endif // NOT AudioBlockHPP
