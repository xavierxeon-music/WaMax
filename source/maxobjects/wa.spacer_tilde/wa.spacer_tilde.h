
#ifndef WaSpacerH
#define WaSpacerH

#include "c74_min.h"
using namespace c74::min;

class Spacer : public object<Spacer>, public sample_operator<1, 2>
{
public:
   MIN_DESCRIPTION{"create spatial audio"};

public:
   Spacer();

public:
   inlet<> input;
   outlet<> leftOutput;
   outlet<> rightOutput;

   samples<2> operator()(sample x);

private:
   sample last;
};

#endif // NOT  WaSpacerH
