#ifndef DataHPP
#define DataHPP

#include "Data.h"

inline const Size& Data::getScreenSize() const
{
   return screenSize;
}

inline Data::Data()
   : screenSize()
   , tpMap()
{
}

#endif // NOT DataHPP
