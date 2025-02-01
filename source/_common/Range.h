#ifndef RangeH
#define RangeH

class Range
{
public:
   Range(const double& min, const double& max);

public:
   double clamp(const double& value) const;
   double operator()(const double& value) const; // same as clamp

   double factor(const double& value) const; // percentage between min and max
   double value(const double& factor) const; // value of percentage

private:
   const double min;
   const double max;
   const double diff;
};

#ifndef RangeHPP
#include "Range.hpp"
#endif // NOT RangeHPP

#endif // NOT RangeH
