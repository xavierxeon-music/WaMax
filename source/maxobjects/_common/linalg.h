#ifndef LinalgH
#define LinalgH

namespace Linalg
{
   class Vector3
   {
   public:
      Vector3(const double& a = 0.0, const double& b = 0.0, const double& c = 0.0);

   public:
      const double& getA() const;
      void setA(const double& value);

      const double& getB() const;
      void setB(const double& value);

      const double& getC() const;
      void setC(const double& value);

   private:
      double a;
      double b;
      double c;
   };
} // namespace Linalg

#ifndef LinalgHPP
#include "linalg.hpp"
#endif // NOT LinalgHPP

#endif // NOT LinalgH
