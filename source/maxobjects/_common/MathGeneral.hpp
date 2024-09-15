#ifndef MathGeneralHPP
#define MathGeneralHPP

#include "MathGeneral.h"

#include <cmath>

inline double Math::deg2Rad(const double& degrees)
{
   const double radians = degrees * (M_PI / 180);
   return radians;
}

inline double Math::rad2Deg(const double& radians)
{
   const double degrees = radians * (180 / M_PI);
   return degrees;
}

#endif // NOT MathGeneralHPP
