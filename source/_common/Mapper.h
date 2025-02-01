#ifndef MapperH
#define MapperH

#include "Range.h"

class Mapper
{
public:
   Mapper(const Range& input, const Range& output = Range(0.0, 1.0));

public:
   double scale(const double& value) const;
   double operator()(const double& value) const; // same as scale

private:
   const Range input;
   const Range output;
};

#ifndef MapperHPP
#include "Mapper.hpp"
#endif // NOT MapperHPP

#endif // NOT MapperH
