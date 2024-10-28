#ifndef MathVector3HPP
#define MathVector3HPP

#include "MathVector3.h"

#include <cmath>

#include "MathGeneral.h"

inline Math::Spherical::Spherical(const double& az, const double& el, const double& radius)
   : az(az)
   , el(el)
   , radius(radius)
{
}

inline bool Math::Spherical::operator<(const Spherical& other) const
{
   if (az < other.az)
      return true;
   else if (az > other.az)
      return false;
   else
      return (el < other.el);
}

//

inline Math::Vector3::Vector3(const double& x, const double& y, const double& z)
   : x(x)
   , y(y)
   , z(z)
{
}

inline bool Math::Vector3::operator==(const Vector3& other) const
{
   if (x != other.x)
      return false;
   else if (y != other.y)
      return false;
   else if (z != other.z)
      return false;

   return true;
}

inline bool Math::Vector3::operator<(const Vector3& other) const
{
   if (x < other.x)
      return true;
   else if (x > other.x)
      return false;
   else if (y < other.y)
      return true;
   else if (y > other.y)
      return false;
   else
      return (z < other.z);
}

inline const double& Math::Vector3::operator[](const int index) const
{
   return data[index];
}

inline double& Math::Vector3::operator[](const int index)
{
   return data[index];
}

inline const double& Math::Vector3::getX() const
{
   return x;
}

inline const double& Math::Vector3::getY() const
{
   return y;
}

inline const double& Math::Vector3::getZ() const
{
   return z;
}

inline Math::Vector3 Math::Vector3::fromSpherical(const Spherical& spherical, const bool fromDegree)

{
   double az = spherical.az;
   double el = spherical.el;

   if (fromDegree)
   {
      az = deg2Rad(az);
      el = deg2Rad(el);
   }

   const double x = spherical.radius * std::sin(el) * std::cos(az);
   const double y = spherical.radius * std::sin(el) * std::sin(az);
   const double z = spherical.radius * std::cos(el);

   return Vector3(x, y, z);
}

inline Math::Spherical Math::Vector3::toSpherical(const bool toDegree) const
{
   const double radius = length();
   if (0.0 == radius)
      return Spherical();

   double el = std::acos(z / radius);

   const double planeRadius = std::sqrt((x * x) + (y * y));
   double az = (planeRadius > 0) ? std::acos(x / planeRadius) : 0.0;
   if (y < 0)
      az = (2 * M_PI) - az;

   if (toDegree)
   {
      az = rad2Deg(az);
      el = rad2Deg(el);
   }

   return Spherical(az, el, radius);
}

inline double Math::Vector3::length() const
{
   const double selfDot = dot(*this);
   return std::sqrt(selfDot);
}

inline Math::Vector3 Math::Vector3::norm() const
{
   const double l = length();
   if (0.0 == l)
      return Vector3(0.0, 0.0, 0.0);

   const double nx = x / l;
   const double ny = y / l;
   const double nz = z / l;

   return Vector3(nx, ny, nz);
}

inline double Math::Vector3::dot(const Vector3& other) const
{
   return (x * other.x) + (y * other.y) + (z * other.z);
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
   const double x = (y * other.z) - (z * other.y);
   const double y = (z * other.x) - (x * other.z);
   const double z = (x * other.y) - (y * other.x);

   return Vector3(x, y, z);
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

//

inline std::ostream& Math::operator<<(std::ostream& out, const Math::Spherical& value)
{
   out << "sperical[" << value.az << ", " << value.el << ", " << value.radius << "]";
   return out;
}

inline std::ostream& Math::operator<<(std::ostream& out, const Math::Vector3& value)
{
   out << "[" << value.x << ", " << value.y << ", " << value.z << "]";
   return out;
}

#endif // NOT MathVector3HPP
