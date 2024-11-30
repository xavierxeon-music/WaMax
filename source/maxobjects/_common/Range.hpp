#ifndef RangeHPP
#define RangeHPP

#include "Range.h"

inline Range::Range(const double& min, const double& max)
   : min(min)
   , max(max)
   , diff(max - min)
{
}

inline double Range::clamp(const double& value) const
{
   if (value < min)
      return min;
   else if (value > max)
      return max;
   else
      return value;
}

inline double Range::operator()(const double& value) const
{
   return clamp(value);
}

inline double Range::factor(const double& value) const
{
   return (value - min) / diff;
}

inline double Range::value(const double& factor) const
{
   return min + (factor * diff);
}

#endif // NOT RangeHPP
