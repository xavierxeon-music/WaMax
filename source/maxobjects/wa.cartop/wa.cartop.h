#ifndef CartOpH
#define CartOpH

#include "c74_min.h"
using namespace c74::min;

class CartOp : public object<CartOp>
{
public:
   MIN_DESCRIPTION{"description"};

public:
   CartOp(const atoms& args = {});
};

#endif // CartOpH
