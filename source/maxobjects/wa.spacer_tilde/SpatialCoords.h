#ifndef SpatialCoordsH
#define SpatialCoordsH

namespace Spatial
{
   struct Coords
   {
      double az = 0;
      double el = 0;

      bool operator<(const Coords& other) const;
   };
} // namespace Spatial

#endif // NOT SpatialCoordsH
