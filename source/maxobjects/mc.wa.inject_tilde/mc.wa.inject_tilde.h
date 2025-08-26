#ifndef McInjectH
#define McInjectH

#include "c74_min.h"
using namespace c74::min;

class McInject : public object<McInject>
{
public:
   MIN_DESCRIPTION{"description"};

public:
   McInject(const atoms& args = {});
};

#endif // McInjectH
