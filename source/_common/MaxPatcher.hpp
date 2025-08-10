#ifndef MaxPatcherHPP
#define MaxPatcherHPP

#include "MaxPatcher.h"

#include <functional>

template <typename ObjectType>
function Max::Patcher::minBind(ObjectType* object, atoms (ObjectType::*functionPointer)(const atoms&, const int))
{
   return std::bind(functionPointer, object, std::placeholders::_1, std::placeholders::_2);
}

template <typename ObjectType>
std::string Max::Patcher::path(ObjectType* object)
{
   using namespace c74;
   max::t_object* max_patch_instance = static_cast<max::t_object*>(object->patcher());
   const std::string patchPath = std::string(max::jpatcher_get_filepath(max_patch_instance)->s_name);

   return patchPath;
}

template <typename ObjectType>
void Max::Patcher::setDirty(ObjectType* object, bool dirty)
{
   using namespace c74;
   max::t_object* max_patch_instance = static_cast<max::t_object*>(object->patcher());
   max::jpatcher_set_dirty(max_patch_instance, dirty ? 1 : 0);
}

#endif // NOT MaxPatcherHPP
