#ifndef PatcherH
#define PatcherH

#include "c74_min.h"
using namespace c74::min;

struct Patcher
{
   template <typename ObjectType>
   static function minBind(ObjectType* object, atoms (ObjectType::*functionPointer)(const atoms&, const int));

   template <typename ObjectType>
   static std::string path(ObjectType* object);

   template <typename ObjectType>
   static void setDirty(ObjectType* object, bool dirty = true);
};

#ifndef PatcherHPP
#include "patcher.hpp"
#endif // NOT PatcherHPP

#endif // NOT PatcherH
