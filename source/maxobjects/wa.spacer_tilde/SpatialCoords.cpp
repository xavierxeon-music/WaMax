#include "SpatialCoords.h"

bool Spatial::Coords::operator<(const Coords& other) const
{
   if (az < other.az)
      return true;
   else if (az > other.az)
      return false;
   else
      return (el < other.el);
}
