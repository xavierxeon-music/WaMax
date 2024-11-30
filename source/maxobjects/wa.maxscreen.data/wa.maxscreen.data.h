#ifndef MaxScreenDataH
#define MaxScreenDataH

#include "c74_min.h"
using namespace c74::min;

class MaxScreenData : public object<MaxScreenData>
{
public:
   MIN_DESCRIPTION{"description"};

public:
   MaxScreenData(const atoms& args = {});
};

#endif // MaxScreenDataH
