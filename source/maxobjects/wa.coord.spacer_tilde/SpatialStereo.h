#ifndef SpatialStereoH
#define SpatialStereoH

namespace Spatial
{
   struct Stereo
   {
      double left = 0.0;
      double right = 0.0;

      Stereo& operator+=(const Stereo& other);
      Stereo& operator*=(const double& value);

      Stereo operator+(const Stereo& other) const;
      Stereo operator*(const double& value) const;

      double max() const;
   };
} // namespace Spatial

#endif // NOT SpatialStereoH
