#ifndef DataHPP
#define DataHPP

#include "Data.h"

inline const ImageSize& Data::getImageSize() const
{
   return screenSize;
}

inline const TouchPoint::Map& Data::getTouchPointMap() const
{
   return tpMap;
}

inline Data::Data()
   : screenSize()
   , tpMap()
{
}

#endif // NOT DataHPP
