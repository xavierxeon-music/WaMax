#ifndef CartOpH
#define CartOpH

#include "c74_min.h"
using namespace c74::min;

namespace Coord
{
   class CartOp : public object<CartOp>
   {
   public:
      MIN_DESCRIPTION{"description"};

   public:
      CartOp(const atoms& args = {});
   };
} // namespace Coord

#endif // CartOpH
