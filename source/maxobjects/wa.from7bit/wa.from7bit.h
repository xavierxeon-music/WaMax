
#ifndef From7BitH
#define From7BitH

#include "c74_min.h"
using namespace c74::min;

class From7Bit : public object<From7Bit>
{
public:
   MIN_DESCRIPTION{"7bit list to int"};

public:
   From7Bit();

private:
   atoms listFunction(const atoms& args, const int inlet);

private:
   inlet<> input;
   outlet<> output;
   message<> listMessage;
};

#endif // NOT From7BitH
