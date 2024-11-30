#include "wa.coord.cartop.h"

Coord::CartOp::CartOp(const atoms& args)
   : object<CartOp>()
   , inputMatrix{this, "matrix", "jit_matrix"}
{
}

MIN_EXTERNAL(Coord::CartOp);
