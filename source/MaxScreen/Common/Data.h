#ifndef DataH
#define DataH

#include <Shared.h>

#include "ImageSize.h"
#include "TouchPoint.h"

using ScreenServer = Shared<"MaxScreen">;

class Data
{
public:
   const ImageSize& getImageSize() const;
   const TouchPoint::Map& getTouchPointMap() const;

protected:
   Data();

protected:
   ImageSize screenSize;
   TouchPoint::Map tpMap;
};

#ifndef DataHPP
#include "Data.hpp"
#endif // NOT DataHPP

#endif // NOT DataH
