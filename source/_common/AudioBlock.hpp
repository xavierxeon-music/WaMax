#ifndef AudioBlockHPP
#define AudioBlockHPP

#include "AudioBlock.h"

inline AudioBlock::AudioBlock(const QString& name, int counter, bool readOnly)
   : SharedData<AudioBlockData>("WaPipe_Block_" + name + "_" + QString::number(counter), readOnly)
   , cursor(0)
{
}

inline AudioBlock::~AudioBlock()
{
   if (!readOnly)
   {
      Guard guard(this); // needs socpe, else crash
      if (dataBlock)
         std::memset(dataBlock->data, 0, BlockSize * sizeof(double));
   }
}

inline void AudioBlock::copyFrom(const double* data, const size_t& size)
{
   Guard guard(this);
   if (dataBlock)
      std::memcpy(dataBlock->data + cursor, data, size * sizeof(double));

   cursor = (cursor + size) % BlockSize;
}

inline void AudioBlock::copyTo(double* data, const size_t& size)
{
   Guard guard(this);
   if (dataBlock)
      std::memcpy(data, dataBlock->data + cursor, size * sizeof(double));

   cursor = (cursor + size) % BlockSize;
}

#endif // NOT AudioBlockHPP
