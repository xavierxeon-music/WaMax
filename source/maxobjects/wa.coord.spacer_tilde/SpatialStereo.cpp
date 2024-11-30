#include "SpatialStereo.h"

Spatial::Stereo& Spatial::Stereo::operator+=(const Stereo& other)
{
   left += other.left;
   right += other.right;

   return *this;
}

Spatial::Stereo& Spatial::Stereo::operator*=(const double& value)
{
   left *= value;
   right *= value;

   return *this;
}

Spatial::Stereo Spatial::Stereo::operator+(const Stereo& other) const
{
   Stereo stereo;
   stereo.left = left + other.left;
   stereo.right = right + other.right;
   return stereo;
}

Spatial::Stereo Spatial::Stereo::operator*(const double& value) const
{
   Stereo stereo;
   stereo.left = left * value;
   stereo.right = right * value;
   return stereo;
}

double Spatial::Stereo::max() const
{
   if (left > right)
      return left;

   return right;
}
