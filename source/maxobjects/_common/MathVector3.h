#ifndef MathVector3H
#define MathVector3H

namespace Math
{
   class Vector3
   {
   public:
      Vector3(const double& a = 0.0, const double& b = 0.0, const double& c = 0.0);

   public:
      bool operator==(const Vector3& other) const;

   public:
      const double& getA() const;
      void setA(const double& value);

      const double& getB() const;
      void setB(const double& value);

      const double& getC() const;
      void setC(const double& value);

      Vector3 sphere2Cart(const bool fromDegree = true);
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
} // namespace Math

#ifndef MathVector3HPP
#include "MathVector3.hpp"
#endif // NOT MathVector3HPP

#endif // NOT LinalgH
