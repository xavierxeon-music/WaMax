#ifndef LinalgH
#define LinalgH

namespace Linalg
{
   double deg2Rad(const double& degrees);
   double rad2Deg(const double& radians);

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

      Vector3 spehreToCart(const bool fromDegree = true);
      Vector3 cart2Sphre(const bool toDegree = true);

      double length() const;

      double dot(const Vector3& other) const;
      double dotAngle(const Vector3& other, const bool toDegree = true) const;

      Vector3 cross(const Vector3& other) const;
      double crossAngle(const Vector3& other, const bool toDegree = true) const;

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
