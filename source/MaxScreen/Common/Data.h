#ifndef DataH
#define DataH

#include <Shared.h>

#include "Size.h"
#include "TouchPoint.h"

using ScreenServer = Shared<"MaxScreen">;

class Data
{
public:
   const Size& getScreenSize() const;

protected:
   Data();

protected:
   Size screenSize;
   TouchPoint::Map tpMap;
};

#ifndef DataHPP
#include "Data.hpp"
#endif // NOT DataHPP

#endif // NOT DataH
