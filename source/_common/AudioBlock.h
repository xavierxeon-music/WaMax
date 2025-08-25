#ifndef AudioBlockH
#define AudioBlockH

#include <QString>

#include <QSharedMemory>

class AudioBlock
{
public:
   AudioBlock(const QString& name, int counter);

public:
   void copyFrom(const double* data, const size_t& size);
   void copyTo(double* data, const size_t& size);

private:
   struct Data
   {
      double data[256]; // needs to be stack memmory, choose suffiently large size
   };

private:
   QSharedMemory sharedMemory;
   Data* sharedData;
};

#ifndef AudioBlockHPP
#include "AudioBlock.hpp"
#endif // NOT AudioBlockHPP

#endif // NOT AudioBlockH
