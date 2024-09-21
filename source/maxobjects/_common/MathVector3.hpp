#ifndef MathVector3HPP
#define MathVector3HPP

#include "MathVector3.h"

#include <cmath>

#include "MathGeneral.h"

inline Math::Vector3::Vector3(const double& a, const double& b, const double& c)
   : a(a)
   , b(b)
   , c(c)
{
}

inline bool Math::Vector3::operator==(const Vector3& other) const
{
   if (a != other.a)
      return false;
   else if (b != other.b)
      return false;
   else if (c != other.c)
      return false;

   return true;
}

inline const double& Math::Vector3::getA() const
{
   return a;
}

inline void Math::Vector3::setA(const double& value)
{
   a = value;
}

inline const double& Math::Vector3::getB() const
{
   return b;
}

inline void Math::Vector3::setB(const double& value)
{
   b = value;
}

inline const double& Math::Vector3::getC() const
{
   return c;
}

inline void Math::Vector3::setC(const double& value)
{
   c = value;
}

inline Math::Vector3 Math::Vector3::sphere2Cart(const bool fromDegree)
{
   double az = a;
   double el = b;

   if (fromDegree)
   {
      az = deg2Rad(az);
      el = deg2Rad(el);
   }

   double radius = c;

   const double x = radius * std::sin(el) * std::cos(az);
   const double y = radius * std::sin(el) * std::sin(az);
   const double z = radius * std::cos(el);

   return Vector3(x, y, z);
}

inline Math::Vector3 Math::Vector3::cart2Sphre(const bool toDegree)
{
   const double radius = length();
   if (0.0 == radius)
      return Vector3(0, 0, 0);

   double el = std::acos(c / radius);

   const double planeRadius = std::sqrt((a * a) + (b * b));
   double az = (planeRadius > 0) ? std::acos(a / planeRadius) : 0.0;
   if (b < 0)
      az = (2 * M_PI) - az;

   if (toDegree)
   {
      az = rad2Deg(az);
      el = rad2Deg(el);
   }

   return Vector3(az, el, radius);
}

inline double Math::Vector3::length() const
{
   const double selfDot = dot(*this);
   return std::sqrt(selfDot);
}

inline double Math::Vector3::dot(const Vector3& other) const
{
   return (a * other.a) + (b * other.b) + (c * other.c);
}

inline double Math::Vector3::dotAngle(const Vector3& other, const bool toDegree) const
{
   const double d = dot(other);
   const double l = length() * other.length();
   double angle = std::acos(d / l);

   if (toDegree)
      angle = rad2Deg(angle);

   return angle;
}

inline Math::Vector3 Math::Vector3::cross(const Vector3& other) const
{
   const double a = (b * other.c) - (c * other.b);
   const double b = (c * other.a) - (a * other.c);
   const double c = (a * other.b) - (b * other.a);

   return Vector3(a, b, c);
}

inline double Math::Vector3::crossAngle(const Vector3& other, const bool toDegree) const
{
   const double c = cross(other).length();
   const double l = length() * other.length();
   double angle = std::asin(c / l);

   if (toDegree)
      angle = rad2Deg(angle);

   return angle;
}

#endif // NOT MathVector3HPP
