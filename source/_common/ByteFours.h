#ifndef ByteFoursH
#define ByteFoursH

#include <iostream>
#include <string>

class ByteFours
{
public:
   enum FoursValue : uint8_t
   {
      O = 0,
      I = 1,
      Z = 2,
      T = 3
   };

public:
   ByteFours(FoursValue a, FoursValue b, FoursValue c, FoursValue d);
   ByteFours(uint8_t byte = 0);

public:
   double value(const FoursValue& index) const;
   static bool isValidSample(double value);
   static ByteFours::FoursValue fromSample(double value);

   operator uint8_t() const;
   std::string toString() const;

private:
   friend std::ostream& operator<<(std::ostream& out, const ByteFours& bf);

private:
   static const uint8_t mask[4];
   FoursValue fours[4];
};

std::ostream& operator<<(std::ostream& out, const ByteFours& bf);

#ifndef ByteFoursHPP
#include "ByteFours.hpp"
#endif // NOT ByteFoursHPP

#endif // NOT ByteFoursH
