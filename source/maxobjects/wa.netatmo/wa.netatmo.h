#ifndef NetAtmoH
#define NetAtmoH

#include "c74_min.h"
using namespace c74::min;

class NetAtmo : public object<NetAtmo>
{
public:
   MIN_DESCRIPTION{"description"};

public:
   NetAtmo(const atoms& args = {});
};

#endif // NetAtmoH
