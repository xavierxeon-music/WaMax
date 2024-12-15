#ifndef MaxScreenDataH
#define MaxScreenDataH

#include "c74_min.h"
using namespace c74::min;

class MaxScreenData : public object<MaxScreenData>
{
public:
   MIN_DESCRIPTION{"max screen data"};

public:
   MaxScreenData(const atoms& args = {});

private:
   atoms doubleClickFunction(const atoms& args, const int inlet);

private:
   message<> doubleClickMessage;
};

#endif // MaxScreenDataH
