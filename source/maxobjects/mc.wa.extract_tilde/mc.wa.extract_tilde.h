#ifndef McExtractH
#define McExtractH

#include "c74_min.h"
using namespace c74::min;

class McExtract : public object<McExtract>
{
public:
   MIN_DESCRIPTION{"description"};

public:
   McExtract(const atoms& args = {});
};

#endif // McExtractH
