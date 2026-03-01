#include "wa.7bit.fromlist.h"

#include <inttypes.h>
#include <vector>

#include <MaxPatcher.h>

From7Bit::From7Bit()
   : object<From7Bit>()
   , input{this, "(list) 7 bit list"}
   , output{this, "(int) integer value"}
   , listMessage{this, "list", "7 bit list.", Max::Patcher::minBind(this, &From7Bit::listFunction)}
{
}

atoms From7Bit::listFunction(const atoms& args, const int inlet)
{
   std::vector<uint8_t> sevenBits;
   for (auto i = 0; i < args.size(); i++)
   {
      const int value = args[i];
      sevenBits.insert(sevenBits.begin(), value);
   }

   long number = 0;
   long power = 1;

   for (const uint8_t& value : sevenBits)
   {
      number += value * power;
      power *= 128;
   }

   output.send(number);
   return {};
}

MIN_EXTERNAL(From7Bit);
