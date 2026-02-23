
#ifndef CoordCart2SphereH
#define CoordCart2SphereH

#include "c74_min.h"
using namespace c74::min;

#include <XXMathVector3.h>

namespace Coord
{
   class Cart2Sphere : public object<Cart2Sphere>
   {
   public:
      MIN_DESCRIPTION{"convert 3d cartesian coordinates to spherical"};

   public:
      Cart2Sphere(const atoms& args = {});

   private:
      atoms xFunction(const atoms& args, const int inlet);
      atoms yFunction(const atoms& args, const int inlet);
      atoms zFunction(const atoms& args, const int inlet);
      atoms listFunction(const atoms& args, const int inlet);
      atoms calculateFunction(const atoms& args, const int inlet);

      void calculate();

   private:
      XX::Math::Vector3 cartesian;

      outlet<> output;
      attribute<bool> asDegrees;
      message<> xMessage;
      message<> yMessage;
      message<> zMessage;
      message<> listMessage;
      message<> bangMessage;
      message<> doubleClick;
   };
} // namespace Coord

#endif // NOT  CoordCart2SphereH
