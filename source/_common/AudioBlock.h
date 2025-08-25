#ifndef AudioBlockH
#define AudioBlockH

#include <QString>

class AudioBlock
{
public:
   size_t timestamp;
   double data[256]; // needs to be stack memmory, choose suffiently large size

public:
   static AudioBlock* create(const QString& name, int counter);
   AudioBlock() = delete;
};

#ifndef AudioBlockHPP
#include "AudioBlock.hpp"
#endif // NOT AudioBlockHPP

#endif // NOT AudioBlockH
