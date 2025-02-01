#ifndef MapperHPP
#define MapperHPP

#include "Mapper.h"

inline Mapper::Mapper(const Range& input, const Range& output)
   : input(input)
   , output(output)
{
}

inline double Mapper::scale(const double& value) const
{
   const double cValue = input.clamp(value);

   const double factor = input.factor(cValue);
   const double ovalue = output.value(factor);

   return ovalue;
}

inline double Mapper::operator()(const double& value) const
{
   return scale(value);
}

#endif // NOT MapperHPP
