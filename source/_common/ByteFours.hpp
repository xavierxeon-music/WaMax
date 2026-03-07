#ifndef ByteFoursHPP
#define ByteFoursHPP

#include "ByteFours.h"

const uint8_t ByteFours::mask[4] = {0xC0, 0x30, 0x0C, 0x03};

inline ByteFours::ByteFours(FoursValue a, FoursValue b, FoursValue c, FoursValue d)
   : fours{a, b, c, d}
{
}

inline ByteFours::ByteFours(uint8_t byte)
   : ByteFours(O, O, O, O)
{
   for (const uint8_t index : {0, 1, 2, 3})
   {
      uint8_t value = byte & mask[index];
      const uint8_t shift = 2 * (3 - index);
      value = (value >> shift);

      fours[index] = static_cast<FoursValue>(value);
   }
}

inline double ByteFours::value(const FoursValue& index) const
{
   switch (fours[index])
   {
      case O:
         return -0.9;
      case I:
         return -0.7;
      case Z:
         return 0.7;
      case T:
         return 0.9;
   }

   return 0.0;
}

bool ByteFours::isValidSample(double value)
{
   if (value < -0.6 || value > 0.6)
      return true;

   return false;
}

ByteFours::FoursValue ByteFours::fromSample(double value)
{
   if (value >= -1.0 && value < -0.8)
      return O;
   else if (value >= -0.8 && value < -0.6)
      return I;
   else if (value >= 0.6 && value < 0.8)
      return Z;
   else if (value >= 0.8 && value <= 1.0)
      return T;
   else
      return O;
}

inline ByteFours::operator uint8_t() const
{
   uint8_t value = 0;

   for (const uint8_t index : {0, 1, 2, 3})
   {
      uint8_t part = static_cast<uint8_t>(fours[index]);
      const uint8_t shift = 2 * (3 - index);
      part = (part << shift);

      value += part;
   }

   return value;
}

std::string ByteFours::toString() const
{
   std::string text = "4x";

   for (const uint8_t index : {0, 1, 2, 3})
   {
      if (ByteFours::O == fours[index])
         text += "O";
      else if (ByteFours::I == fours[index])
         text += "I";
      else if (ByteFours::Z == fours[index])
         text += "Z";
      else if (ByteFours::T == fours[index])
         text += "T";
   }

   return text;
}

//

std::ostream& operator<<(std::ostream& out, const ByteFours& bf)
{
   out << bf.toString();
   return out;
}

#endif // NOT ByteFoursHPP
