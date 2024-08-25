#ifndef WaPackageCommonH
#define WaPackageCommonH

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

#ifndef WaPackageCommonHPP
#include "common.hpp"
#endif // NOT WaPackageCommonHPP

#endif // NOT WaPackageCommonH
