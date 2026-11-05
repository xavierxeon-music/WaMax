#ifndef GrowlH
#define GrowlH

#include "c74_min.h"
using namespace c74::min;

class Growl : public object<Growl>
{
public:
   MIN_DESCRIPTION{"description"};

public:
   Growl(const atoms& args = {});
};

#endif // GrowlH
