#ifndef LinalgHPP
#define LinalgHPP

#include <cmath>

#include "linalg.h"

double Linalg::deg2Rad(const double& degrees)
{
   const double radians = degrees * (M_PI / 180);
   return radians;
}

double Linalg::rad2Deg(const double& radians)
{
   const double degrees = radians * (180 / M_PI);
   return degrees;
}

Linalg::Vector3::Vector3(const double& a, const double& b, const double& c)
   : a(a)
   , b(b)
   , c(c)
{
}

const double& Linalg::Vector3::getA() const
{
   return a;
}

void Linalg::Vector3::setA(const double& value)
{
   a = value;
}

const double& Linalg::Vector3::getB() const
{
   return b;
}

void Linalg::Vector3::setB(const double& value)
{
   b = value;
}

const double& Linalg::Vector3::getC() const
{
   return c;
}

void Linalg::Vector3::setC(const double& value)
{
   c = value;
}

Linalg::Vector3 Linalg::Vector3::spehreToCart(const bool fromDegree)
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

Linalg::Vector3 Linalg::Vector3::cart2Sphre(const bool toDegree)
{
   const double radius = length();
   if (0.0 == radius)
      return Vector3(0, 0, 0);

   double el = std::acos(c / radius);

   const double planeRadius = std::sqrt(a * a + b * b);
   double az = std::acos(a / planeRadius);
   if (b < 0)
      az *= -1.0;

   if (toDegree)
   {
      az = rad2Deg(az);
      el = rad2Deg(el);
   }

   return Vector3(az, el, radius);
}

double Linalg::Vector3::length() const
{
   const double selfDot = dot(*this);
   return std::sqrt(selfDot);
}

double Linalg::Vector3::dot(const Vector3& other) const
{
   return (a * other.a) + (b * other.b) + (c * other.c);
}

double Linalg::Vector3::dotAngle(const Vector3& other, const bool toDegree) const
{
   const double d = dot(other);
   const double l = length() * other.length();
   double angle = std::acos(d / l);

   if (toDegree)
      angle = rad2Deg(angle);

   return angle;
}

Linalg::Vector3 Linalg::Vector3::cross(const Vector3& other) const
{
   const double a = (b * other.c) - (c * other.b);
   const double b = (c * other.a) - (a * other.c);
   const double c = (a * other.b) - (b * other.a);

   return Vector3(a, b, c);
}

double Linalg::Vector3::crossAngle(const Vector3& other, const bool toDegree) const
{
   const double c = cross(other).length();
   const double l = length() * other.length();
   double angle = std::asin(c / l);

   if (toDegree)
      angle = rad2Deg(angle);

   return angle;
}

#endif // NOT LinalgHPP