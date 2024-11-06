#ifndef FileAbstractH
#define FileAbstractH

#include "PatchStructure.h"

namespace Package
{
   class Info;
}

namespace File
{
   class Abstract
   {
   public:
      Abstract(Patch::Structure* structure, const Package::Info* info);

   public:
      virtual void read(const Patch::Info& patchInfo);
      virtual void write(const Patch::Info& patchInfo) = 0;
      virtual QString getFilePath(const Patch::Info& patchInfo) = 0;

   protected:
      Patch::Structure* structure;
      const Package::Info* info;
   };
} // namespace File

#endif // NOT FileAbstractH
