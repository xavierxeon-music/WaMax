#ifndef MaxPatcherH
#define MaxPatcherH

#include "c74_min.h"
using namespace c74::min;

namespace Max
{
   struct Patcher
   {
      template <typename ObjectType>
      static function minBind(ObjectType* object, atoms (ObjectType::*functionPointer)(const atoms&, const int));

      template <typename ObjectType>
      static std::string path(ObjectType* object);

      template <typename ObjectType>
      static void setDirty(ObjectType* object, bool dirty = true);
   };
} // namespace Max

#ifndef MaxPatcherHPP
#include "MaxPatcher.hpp"
#endif // NOT MaxPatcherHPP

#endif // NOT MaxPatcherH
