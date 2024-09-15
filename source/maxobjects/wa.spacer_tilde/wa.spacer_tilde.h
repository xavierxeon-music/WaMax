
#ifndef WaSpacerH
#define WaSpacerH

#include "c74_min.h"
using namespace c74::min;

#include <MathVector3.h>

#include "SpatialRingBuffer.h"

class Spacer : public object<Spacer>, public sample_operator<1, 2>
{
public:
   MIN_DESCRIPTION{"create spatial audio"};

public:
   Spacer(const atoms& args = {});

public:
   samples<2> operator()(sample x);

private:
   atoms xFunction(const atoms& args, const int inlet);
   atoms yFunction(const atoms& args, const int inlet);
   atoms zFunction(const atoms& args, const int inlet);
   atoms listFunction(const atoms& args, const int inlet);

private:
   Math::Vector3 cartesian;
   Math::Vector3 spherical;
   Spatial::RingBuffer buffer;

   inlet<> input;
   outlet<> leftOutput;
   outlet<> rightOutput;
   attribute<bool> asDegrees;
   message<> xMessage;
   message<> yMessage;
   message<> zMessage;
   message<> listMessage;
};

#endif // NOT  WaSpacerH
