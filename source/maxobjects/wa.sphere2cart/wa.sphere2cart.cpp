#include "wa.sphere2cart.h"

#include <inttypes.h>
#include <vector>

#include <patcher.h>

Sphere2Cart::Sphere2Cart()
   : object<Sphere2Cart>()
   , input{this, "(list) 7 bit list"}
   , output{this, "(int) integer value"}
   , listMessage{this, "list", "7 bit list.", Patcher::minBind(this, &Sphere2Cart::listFunction)}
{
}

atoms Sphere2Cart::listFunction(const atoms& args, const int inlet)
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

MIN_EXTERNAL(Sphere2Cart);
