#include "wa.7bit.tolist.h"

#include <inttypes.h>
#include <vector>

#include <MaxPatcher.h>

To7Bit::To7Bit()
   : object<To7Bit>()
   , input{this, "(int) values to To7Bit"}
   , output{this, "(list) 7 bit list"}
   , intMessage{this, "int", "integer value.", Max::Patcher::minBind(this, &To7Bit::intFunction)}
{
}

atoms To7Bit::intFunction(const atoms& args, const int inlet)
{
   const long intValue = args[0];
   static const char maxSize = sizeof(long);

   std::vector<uint8_t> sevenBits;
   int maxValue = 1;
   for (char i = 0; i < maxSize; i++)
   {
      if (intValue < maxValue)
         continue;

      long value = intValue >> (7 * i);
      value = value & 127;
      maxValue *= 128;
      sevenBits.insert(sevenBits.begin(), value);
   }

   atoms result(sevenBits.size());
   for (char i = 0; i < sevenBits.size(); i++)
   {
      result[i] = sevenBits[i];
   }

   output.send(result);
   return {};
}

MIN_EXTERNAL(To7Bit);
