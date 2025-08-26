#ifndef AudioBlockH
#define AudioBlockH

#include <QString>

#include <SharedData.h>

constexpr static size_t BlockSize = 2048;
struct AudioBlockData
{
   double data[BlockSize]; // needs to be stack memmory, choose suffiently large size
};

class AudioBlock : public SharedData<AudioBlockData>
{
public:
   AudioBlock(const QString& name, int counter, bool readOnly);
   ~AudioBlock() override;

public:
   void copyFrom(const double* data, const size_t& size);
   void copyTo(double* data, const size_t& size);

private:
   size_t cursor;
};

#ifndef AudioBlockHPP
#include "AudioBlock.hpp"
#endif // NOT AudioBlockHPP

#endif // NOT AudioBlockH
