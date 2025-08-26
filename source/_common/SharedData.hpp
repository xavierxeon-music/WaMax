#ifndef SharedDataHPP
#define SharedDataHPP

#include "SharedData.h"

template <typename DataBlock>
SharedData<DataBlock>::SharedData(const QString& uniqueName, bool readOnly)
   : dataBlock(nullptr)
   , sharedMemory(uniqueName)
   , readOnly(readOnly)
   , errorString()
{
   const QSharedMemory::AccessMode mode = readOnly ? QSharedMemory::ReadOnly : QSharedMemory::ReadWrite;
   if (sharedMemory.create(sizeof(DataBlock), mode))
   {
      dataBlock = reinterpret_cast<DataBlock*>(sharedMemory.data());
      return;
   }

   errorString = sharedMemory.errorString();
   if (QSharedMemory::AlreadyExists == sharedMemory.error())
   {
      sharedMemory.attach(mode);
      if (0 == sharedMemory.size())
      {
         sharedMemory.detach();
         sharedMemory.create(sizeof(DataBlock), mode);
      }
      errorString = sharedMemory.errorString();
      dataBlock = reinterpret_cast<DataBlock*>(sharedMemory.data());
   }
}

template <typename DataBlock>
inline SharedData<DataBlock>::~SharedData()
{
   if (!readOnly)
   {
      sharedMemory.lock();
      clear(dataBlock);
      sharedMemory.unlock();
   }
   sharedMemory.detach();
}

template <typename DataBlock>
const QString& SharedData<DataBlock>::getErrorString() const
{
   return errorString;
}

#endif // NOT SharedDataHPP
