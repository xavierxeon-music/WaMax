#ifndef MathGeneralH
#define MathGeneralH

namespace Math
{
   double deg2Rad(const double& degrees);
   double rad2Deg(const double& radians);

   bool signChange(const double& value1, const double& value2);

} // namespace Math

#ifndef MathGeneralHPP
#include "MathGeneral.hpp"
#endif // NOT MathGeneralHPP

#endif // NOT MathGeneralH
