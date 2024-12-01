#ifndef DataHPP
#define DataHPP

#include "Data.h"

inline const ImageSize& Data::getImageSize() const
{
   return screenSize;
}

inline Data::Data()
   : screenSize()
   , tpMap()
{
}

#endif // NOT DataHPP
