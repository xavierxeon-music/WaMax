#ifndef WaTo7BitH
#define WaTo7BitH

#include "c74_min.h"
using namespace c74::min;

class To7Bit : public object<To7Bit>
{
public:
   MIN_DESCRIPTION{"int to 7bit list"};

public:
   To7Bit();

public:
   inlet<> input;
   outlet<> output;
   message<> intMessage;

private:
   atoms intFunction(const atoms& args, const int inlet);
};

#endif // NOT WaTo7BitH
