
#ifndef Sphere2CartH
#define Sphere2CartH

#include "c74_min.h"
using namespace c74::min;

#include <XXMathVector3.h>

namespace Coord
{
   class Sphere2Cart : public object<Sphere2Cart>
   {
   public:
      MIN_DESCRIPTION{"convert spherical coordinates to 3d cartesian"};

   public:
      Sphere2Cart(const atoms& args = {});

   private:
      atoms azFunction(const atoms& args, const int inlet);
      atoms elFunction(const atoms& args, const int inlet);
      atoms radiusFunction(const atoms& args, const int inlet);
      atoms listFunction(const atoms& args, const int inlet);
      atoms calculateFunction(const atoms& args, const int inlet);
      void calculate();

   private:
      XX::Math::Spherical spherical;

      outlet<> output;
      attribute<bool> asDegrees;
      message<> azMessage;
      message<> elMessage;
      message<> radiusMessage;
      message<> listMessage;
      message<> bangMessage;
      message<> doubleClick;
   };
} // namespace Coord

#endif // NOT Sphere2CartH
