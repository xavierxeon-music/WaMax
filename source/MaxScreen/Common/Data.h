#ifndef DataH
#define DataH

#include <Shared.h>

#include "Size.h"
#include "TouchPoint.h"

using ScreenServer = Shared<"MaxScreen">;

struct Data
{
   Size screenSize;
   TouchPoint::Map tpMap;

   Data();
};

#ifndef DataHPP
#include "Data.hpp"
#endif // NOT DataHPP

#endif // NOT DataH
