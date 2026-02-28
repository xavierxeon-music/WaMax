
#ifndef SpacerH
#define SpacerH

#include "c74_min.h"
using namespace c74::min;

#include <XXLinalgVector3.h>

#include "SpatialRingBuffer.h"

namespace Coord
{
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
      XX::Linalg::Vector3 cartesian;
      XX::Linalg::Spherical spherical;
      Spatial::RingBuffer buffer;

      inlet<> input;
      outlet<> leftOutput;
      outlet<> rightOutput;
      attribute<bool> asDegrees;
      attribute<bool> active;
      message<> xMessage;
      message<> yMessage;
      message<> zMessage;
      message<> listMessage;
   };
} // namespace Coord

#endif // NOT SpacerH
