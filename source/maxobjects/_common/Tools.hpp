#ifndef ToolsHPP
#define ToolsHPP

#include "Tools.h"

//  range

inline Tools::Range::Range(const double& min, const double& max)
   : min(min)
   , max(max)
   , diff(max - min)
{
}

inline double Tools::Range::clamp(const double& value) const
{
   if (value < min)
      return min;
   else if (value > max)
      return max;
   else
      return value;
}

inline double Tools::Range::operator()(const double& value) const
{
   return clamp(value);
}

inline double Tools::Range::factor(const double& value) const
{
   return (value - min) / diff;
}

inline double Tools::Range::value(const double& factor) const
{
   return min + (factor * diff);
}

// mapper

inline Tools::Mapper::Mapper(const Range& input, const Range& output)
   : input(input)
   , output(output)
{
}

inline double Tools::Mapper::scale(const double& value) const
{
   const double cValue = input.clamp(value);

   const double factor = input.factor(cValue);
   const double ovalue = output.value(factor);

   return ovalue;
}

inline double Tools::Mapper::operator()(const double& value) const
{
   return scale(value);
}

#endif // NOT ToolsHPP
